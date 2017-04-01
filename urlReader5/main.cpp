#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>
#include <boost/scoped_ptr.hpp>

typedef boost::function<void(const std::string &)> writeDelegate;
typedef boost::function<void(bool)> connectionDelegate;

class urlReader
{
public:
	urlReader(boost::shared_ptr<boost::asio::io_service> io, writeDelegate dlg)
		: resolver_	(*io)
		, socket_	(*io)
		, strand_	(*io)
		, answered_	(false)
		, dlg_		(dlg) {}

	void connect(const std::string & host, int port, const std::string & path)
	{
		auto query = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
			new boost::asio::ip::tcp::resolver::query(
				host, boost::lexical_cast<std::string>(port)));

		// build the query
		std::ostream request_stream(&request_);
		request_stream << "GET /";
		request_stream << path;
		request_stream << " HTTP/1.1\r\n";
		request_stream << "Host: " << host << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		resolver_.async_resolve(*query, strand_.wrap(
			boost::bind(&urlReader::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator)));
	}

	void urlReader::getStream()
	{
		if (!answered_)
		{
			boost::chrono::high_resolution_clock timer;
			boost::chrono::time_point<boost::chrono::high_resolution_clock> start = timer.now();
			boost::mutex::scoped_lock lock(ioMutex_);

			// should be useless...
			while (!answered_ && boost::chrono::duration_cast<boost::chrono::milliseconds>(
				timer.now() - start).count() < 10000)
			{
				condition_.wait(lock);
			}
		}

		dlg_(content_.str());
	}

private:
	// client callbacks
	void urlReader::handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
				strand_.wrap(boost::bind(&urlReader::handle_connect, this,
					boost::asio::placeholders::error)));
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}
	void urlReader::handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			boost::asio::async_write(socket_, request_,
				strand_.wrap(boost::bind(&urlReader::handle_write_request, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}

	void urlReader::handle_write_request	(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			boost::asio::async_read_until(socket_, response_, "\r\n",
				strand_.wrap(boost::bind(&urlReader::handle_read_status_line, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}
	void urlReader::handle_read_status_line	(const boost::system::error_code& err, size_t bytes_transferred)
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
				boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
					strand_.wrap(boost::bind(&urlReader::handle_read_headers, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else if (status_code == 302)							// redirection
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
					strand_.wrap(boost::bind(&urlReader::handle_redirection, this,
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
	void urlReader::handle_redirection		(const boost::system::error_code& err, size_t bytes_transferred)
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
	void urlReader::handle_read_headers		(const boost::system::error_code& err, size_t bytes_transferred)
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
				header_ << h;
			}

			// Start reading remaining data until EOF.
			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				strand_.wrap(boost::bind(&urlReader::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			answered_ = true; condition_.notify_one();
		}
	}
	void urlReader::handle_read_content		(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Continue reading remaining data until EOF.
			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				strand_.wrap(boost::bind(&urlReader::handle_read_content, this,
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
	writeDelegate dlg_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;
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

	// for async controls
	boost::mutex 				ioMutex_;
	boost::condition_variable 	condition_;
	boost::atomic<bool> 		answered_;
};

class worker
{
public:
	worker(writeDelegate callback)
		: callback_(callback)
		, counter_(1) 
	{
	
	}

	~worker() {};
	void work()
	{
		// initialize and run the service
		boost::shared_ptr<boost::asio::io_service> io(
			new boost::asio::io_service);

		boost::unique_ptr<boost::asio::io_service::work> ioTask = boost::unique_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(*io));

		// runs in a separate thread
		boost::thread t1([&] { io->run(); });

		boost::shared_ptr<urlReader> reader(new urlReader(io, callback_));
		reader->connect("www.google.com", 80, "");

		reader->getStream();

		ioTask.reset();

		t1.join();
	}
private:
	writeDelegate callback_;
	int counter_;
};

class service 
{
public:
	class writer
	{
	public:
		void write(const std::string & s)
		{
			boost::mutex::scoped_lock lock(m_);
			std::cout << s << std::endl;
		};
	private:
		boost::mutex m_;
	};

	service(int n) : n_(n) {}

	void start()
	{
		writeDelegate f(boost::bind(&writer::write, &w_, _1));

		for (int i = 0; i < n_; i++)
		{
			boost::shared_ptr<worker> w(new worker(f));
			threadPool_.push_back(boost::shared_ptr<boost::thread>(new boost::thread([&w] { w->work(); })));
			workers_.push_back(w);
		}

		for (auto & i : threadPool_) i->join();
	}
private:

	writer w_;
	std::vector<boost::shared_ptr<worker>> workers_;
	std::vector<boost::shared_ptr<boost::thread>> threadPool_;
	int n_;
};
 
int main()
{
	service srv(2);
	srv.start();
	return 0;
}