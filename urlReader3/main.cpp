#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>
#include <boost/scoped_ptr.hpp>

// maintains the connection to a host
class connector
{
public:
	connector(boost::shared_ptr<boost::asio::io_service> io, const std::string & host, int port)
		: io_		(io		)
		, socket_	(*io_	)
		, resolver_	(*io_	)
		, strand_	(*io_	)
		, host_		(host	)
		, port_		(port	) 
	{
		auto query = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
			new boost::asio::ip::tcp::resolver::query(
				host_, boost::lexical_cast<std::string>(port_)));

		resolver_.async_resolve(*query, strand_.wrap(
			boost::bind(&connector::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator))); 
	}

	boost::shared_ptr<boost::asio::io_service> service() { return io_; }
	boost::asio::ip::tcp::socket & socket() { return socket_; }
	const std::string & host() const { return host_; }
	bool connected() const { return connected_; }

private:
	void connector::handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
				strand_.wrap(boost::bind(&connector::handle_connect, this,
					boost::asio::placeholders::error)));
		}
		else
		{
			connected_ = false;
		}
	}

	void connector::handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			std::cout << "connection successful !" << std::endl;
		}
		else
		{
			connected_ = false;
		}
	}

	boost::shared_ptr<boost::asio::io_service> io_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::strand strand_;
	boost::asio::ip::tcp::socket socket_;

	bool connected_;
	std::string host_;
	int port_;
};

// read a page content
class queryReader
{
public:
	queryReader(boost::shared_ptr<connector> cnx, const std::string & path, int timeout = 5000)
		: cnx_(cnx)
		, strand_(*cnx->service())
		, timeout_(timeout)
		, answered_(false)
	{
		// build the query
		std::ostream request_stream(&request_);
		request_stream << "GET /";
		request_stream << path;
		request_stream << " HTTP/1.1\r\n";
		request_stream << "Host: " << cnx->host() << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		if (cnx_->connected())
		{
			boost::asio::async_write(cnx_->socket(), request_,
				strand_.wrap(boost::bind(&queryReader::handle_write_request, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
	}

	std::string queryReader::getStream()
	{
		if (!answered_)
		{
			boost::chrono::high_resolution_clock timer;
			boost::chrono::time_point<boost::chrono::high_resolution_clock> start = timer.now();
			boost::mutex::scoped_lock lock(ioMutex_);

			// should be useless...
			while (!answered_ && boost::chrono::duration_cast<boost::chrono::milliseconds>(
				timer.now() - start).count() < timeout_)
			{
				condition_.wait(lock);
			}
		}

		return content_.str();
	}

private:
	// client callbacks
	void queryReader::handle_write_request(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			boost::asio::async_read_until(cnx_->socket(), response_, "\r\n",
				strand_.wrap(boost::bind(&queryReader::handle_read_status_line, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}
	void queryReader::handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Check that the response is OK.
			std::istream response_stream(&response_);
			std::string http_version;
			response_stream >> http_version;

			unsigned int status_code;
			response_stream >> status_code;

			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				answered_ = true; condition_.notify_one();
				return;
			}

			if (status_code == 200)
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_->socket(), response_, "\r\n\r\n",
					strand_.wrap(boost::bind(&queryReader::handle_read_headers, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else if (status_code == 302)							// redirection
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_->socket(), response_, "\r\n\r\n",
					strand_.wrap(boost::bind(&queryReader::handle_redirection, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else
			{
				answered_ = true; condition_.notify_one();
				return;
			}
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}
	void queryReader::handle_redirection(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&response_);

			// unload the bufstream in header_ until we reached the content (standalone empty line)
			// TODO: we may reach the end of the packet before the end of the header
			std::string h; while (std::getline(response_stream, h))
			{
				std::cout << h << std::endl;
			}

			answered_ = false; condition_.notify_one();
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}
	void queryReader::handle_read_headers(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&response_);

			// unload the bufstream in header_ until we reached the content (standalone empty line)
			// TODO: we may reach the end of the packet before the end of the header
			std::string h; while (std::getline(response_stream, h) && h != "\r")
			{
				// TODO: use regex for the space
				if (h.find("transfer-encoding: chunked") != std::string::npos) chunked_ = true;
				std::cout << h << std::endl;
				header_ << h;
			}

			// Start reading remaining data until EOF.
			boost::asio::async_read(cnx_->socket(), response_,
				boost::asio::transfer_at_least(1),
				strand_.wrap(boost::bind(&queryReader::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}
	void queryReader::handle_read_content(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Continue reading remaining data until EOF.
			boost::asio::async_read(cnx_->socket(), response_,
				boost::asio::transfer_at_least(1),
				strand_.wrap(boost::bind(&queryReader::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else if (err == boost::asio::error::eof)
		{
			if (chunked_)
			{
				// unchunked here
			}
			else
			{
				content_ << &response_;
			}

			success_ = true; answered_ = true; condition_.notify_one();
		}
		else if (err != boost::asio::error::eof)
		{
			answered_ = true; condition_.notify_one();
		}
	}

private:
	boost::shared_ptr<connector> cnx_;
	boost::asio::strand strand_;

	std::stringstream content_;
	std::stringstream header_;

	boost::asio::streambuf request_;
	boost::asio::streambuf response_;

	bool ready_;
	bool success_;

	// for chucks management
	bool chunked_;
	int chunckSize_;

	// timeout
	int timeout_;

	// for async controls
	boost::mutex 				ioMutex_;
	boost::condition_variable 	condition_;
	boost::atomic<bool> 		answered_;
};

bool finished = false;
boost::mutex m;
boost::condition_variable cv;

// multithreaded version of queryReader1
int main()
{
	int size = 10;

	// initialize and run the service
	boost::shared_ptr<boost::asio::io_service> io(
		new boost::asio::io_service);

	boost::scoped_ptr<boost::asio::io_service::work> work(
		new boost::asio::io_service::work(*io));

	// runs in a separate thread
	boost::thread t([&] { io->run(); });

	boost::shared_ptr<connector> c(new connector(io, "www.google.com", 80));

	std::vector<boost::shared_ptr<queryReader>> readers;

	for (int i = 0; i < size; i++)
	{
		readers.push_back(boost::shared_ptr<queryReader>(
			new queryReader(c, "")));
	}

	for (int i = 0; i < size; i++)
	{
		auto str = readers[i]->getStream();
	}

	// exit
	boost::unique_lock<boost::mutex> lk(m);

	while (!finished) cv.wait(lk);

	work.reset();

	t.join();

	return 0;
}