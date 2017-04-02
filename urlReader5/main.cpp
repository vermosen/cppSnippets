#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>
#include <boost/move/unique_ptr.hpp>

#include "threadUtil.hpp"
#include "workerImpl.hpp"
typedef boost::function<void(const std::string &)> writeDelegate;
typedef boost::function<void(bool)> connectionDelegate;

class urlReader
{
public:
	urlReader(boost::shared_ptr<boost::asio::io_service> io, writeDelegate write, connectionDelegate cnx)
		: resolver_	(*io)
		, socket_	(*io)
		, strand_	(*io)
		, write_	(write)
		, cnx_		(cnx) {}

	void connect(const std::string & host, int port)
	{
		host_ = host; port_ = port;

		auto query = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
			new boost::asio::ip::tcp::resolver::query(
				host_, boost::lexical_cast<std::string>(port)));

		resolver_.async_resolve(*query, strand_.wrap(
			boost::bind(&urlReader::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator)));
	}

	void urlReader::getStream(const std::string & path)
	{
		// build the query
		std::ostream request_stream(&request_);
		request_stream << "GET /";
		request_stream << path;
		request_stream << " HTTP/1.1\r\n";
		request_stream << "Host: " << host_ << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		boost::asio::async_write(socket_, request_,
			strand_.wrap(boost::bind(&urlReader::handle_write_request, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
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
			// TODO: error handler
			cnx_(false);
		}
	}
	void urlReader::handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			cnx_(true);			
		}
		else
		{
			// TODO: error handler
			cnx_(false);
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
				return;
			}
		}
		else
		{

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
		}
		else
		{

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
			content_ << &response_;
			write_(content_.str());
		}
		else if (err != boost::asio::error::eof)
		{

		}
	}

private:
	writeDelegate write_;
	connectionDelegate cnx_;

	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::strand strand_;

	std::string host_;
	int port_;

	std::stringstream content_;
	std::stringstream header_;

	boost::asio::streambuf request_;
	boost::asio::streambuf response_;

	bool ready_;
	bool success_;

	// for chucks management
	bool chunked_;
	int chunckSize_;
};

class worker : public workerImpl<worker>
{
public:
	worker(writeDelegate write, const std::string name = "worker")
		: workerImpl(name)
		, write_(write)
		, counter_(1)
	{
		io_ = boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service);
	}

	~worker() { io_.reset(); };

	void work()
	{
		// initialize and run the service
		ioTask_ = boost::shared_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(*io_));

		// runs in a separate thread
		t_ = boost::unique_ptr<boost::thread>(new boost::thread([&] { io_->run(); }));
		setThreadName(t_->get_id(), "io runner");

		connectionDelegate c(boost::bind(&worker::connect_callback, this, _1));

		reader_ = boost::shared_ptr<urlReader>(new urlReader(io_, write_, c));
		reader_->connect("www.google.com", 80);

		// barrier
		boost::unique_lock<boost::mutex> lk(m_);
		while (!terminate_) cv_.wait(lk);
		ioTask_.reset();
		t_->join();
	}

private:
	void connect_callback(bool result)
	{
		if (result == true)
		{
			write_("connection successfull");
			reader_->getStream("");
		}
		else
		{
			write_("error");
		}
	}

	boost::unique_ptr<boost::thread> t_;	
	boost::condition_variable cv_;
	bool terminate_;
	boost::mutex m_;

	boost::shared_ptr<boost::asio::io_service> io_;
	boost::shared_ptr<boost::asio::io_service::work> ioTask_;
	boost::shared_ptr<urlReader> reader_;
	writeDelegate write_;
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

		workers_.resize(n_);

		for (int i = 0; i < n_; i++)
		{
			std::string id = boost::lexical_cast<std::string>(i);
			workers_[i] = boost::shared_ptr<worker>(new worker(f, "worker " + id));
		}
		
		for (auto & i : workers_) i->start();
		for (auto & i : workers_) i->join();
	}
private:

	writer w_;
	std::vector<boost::shared_ptr<worker>> workers_;
	int n_;
};
 
int main()
{
	service srv(3 );
	srv.start();
	return 0;
}