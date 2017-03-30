#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "threadUtil.hpp"

// maintains the connection to a host
class connector : public boost::enable_shared_from_this<connector>
{
public:
	connector(	boost::asio::io_service & io, 
				const std::string & host, int port, 
				boost::function<void()> callback)
		: io_		(io			)
		, socket_	(io			)
		, resolver_	(io			)
		, host_		(host		)
		, port_		(port		)
		, callback_ (callback	)
	{
		auto query = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
			new boost::asio::ip::tcp::resolver::query(
				host_, boost::lexical_cast<std::string>(port_)));

		resolver_.async_resolve(*query, 
			boost::bind(&connector::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator)); 
	}

	~connector() {}

	boost::asio::ip::tcp::socket & socket() { return socket_; }
	const std::string & host() const { return host_; }
	bool connected() const { return connected_; }

private:
	void connector::handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			boost::asio::async_connect(socket_, endpoint_iterator,
				boost::bind(&connector::handle_connect, this,
					boost::asio::placeholders::error));
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
			connected_ = true;

			std::cout 
				<< "connection to host " 
				<< boost::lexical_cast<std::string>(socket_.remote_endpoint())
				<< " successful !" << std::endl;

			callback_();
		}
		else
		{
			connected_ = false;
		}
	}

	boost::asio::io_service & io_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;
	boost::function<void()> callback_;
	bool connected_;
	std::string host_;
	int port_;
};

// cycles through the files
class worker
{
public:
	worker(boost::asio::io_service & io, connector & cnx, boost::function<void(const std::string &)> & callback,  int timeout = 5000)
		: io_		(io			)	
		, cnx_		(cnx		)
		, strand_	(io			)
		, timeout_	(timeout	)
		, stop_		(false		)
		, callback_ (callback	)
		, counter_	(1			) 
	{}

	void work()	// start the download loop
	{
		// start the runner
		send();
		boost::unique_lock<boost::mutex> lk(ioMutex_);
		while (!stop_) condition_.wait(lk);
	}

	void send()
	{
		boost::asio::streambuf request;

		// build the query
		std::ostream request_stream(&request);
		request_stream << "GET /" << "" /* put the path here */;
		request_stream << " HTTP/1.1\r\n";
		request_stream << "Host: " << cnx_.host() << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		if (cnx_.connected())
		{
			boost::asio::async_write(cnx_.socket(), request,
				strand_.wrap(boost::bind(&worker::handle_write_request, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
	}

private:
	// client callbacks
	void worker::handle_write_request(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			boost::asio::async_read_until(cnx_.socket(), response_, "\r\n",
				strand_.wrap(boost::bind(&worker::handle_read_status_line, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			stop_ = true; condition_.notify_one();
		}
	}
	void worker::handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred)
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
				stop_ = true; condition_.notify_one();
				return;
			}

			if (status_code == 200)
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_.socket(), response_, "\r\n\r\n",
					strand_.wrap(boost::bind(&worker::handle_read_headers, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else if (status_code == 302)							// redirection
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_.socket(), response_, "\r\n\r\n",
					strand_.wrap(boost::bind(&worker::handle_redirection, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			// TODO: in case of error 40x, timer + call send 
			else
			{
				stop_ = true; condition_.notify_one();
				return;
			}
		}
		else
		{
			stop_ = true; condition_.notify_one();
		}
	}
	void worker::handle_redirection(const boost::system::error_code& err, size_t bytes_transferred)
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

			stop_ = true; condition_.notify_one();
		}
		else
		{
			stop_ = true; condition_.notify_one();
		}
	}
	void worker::handle_read_headers(const boost::system::error_code& err, size_t bytes_transferred)
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
			boost::asio::async_read(cnx_.socket(), response_,
				boost::asio::transfer_at_least(1),
				strand_.wrap(boost::bind(&worker::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			stop_ = true; condition_.notify_one();
		}
	}

	void worker::handle_read_content(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Continue reading remaining data until EOF.
			boost::asio::async_read(cnx_.socket(), response_,
				boost::asio::transfer_at_least(1),
				strand_.wrap(boost::bind(&worker::handle_read_content, this,
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
				callback_("recieved reply number " + boost::lexical_cast<std::string>(++counter_));
				if (!stop_) this->send();
			}
		}
		else if (err != boost::asio::error::eof)
		{
			stop_ = true; condition_.notify_one();
		}
	}

private:
	connector & cnx_;
	boost::asio::strand strand_;
	boost::asio::io_service & io_;
	boost::function<void(const std::string &)> & callback_;

	std::stringstream content_;
	std::stringstream header_;

	boost::asio::streambuf response_;

	// for chucks management
	bool chunked_;
	int chunckSize_;

	// timeout
	int timeout_;

	// for async controls
	boost::mutex 				ioMutex_	;
	boost::condition_variable 	condition_	;
	bool 						stop_		;
	int							counter_	;
};

class service
{
public:
	service()
		: io_() {}

	void run()
	{
		// initialize and run the service
		boost::scoped_ptr<boost::asio::io_service::work> work(
			new boost::asio::io_service::work(io_));

		// runs in a separate thread
		boost::thread t([&] { io_.run(); });
		setThreadName(t.get_id(), "service runner");
		auto f = boost::function<void()>(boost::bind(&service::connectWorkers, this));

		// connect
		connector_ = boost::shared_ptr<connector>(new connector(io_, "www.google.com", 80, f));

		// wait for exit
		boost::unique_lock<boost::mutex> lk(m_);
		while (!finished_) cv_.wait(lk);
		work.reset();
		t.join();
	}

private:
	void connectWorkers()
	{
		int size = 1;

		auto f = boost::function<void(const std::string &)>(boost::bind(&service::callback, this, _1));

		for (int i = 0; i < size; i++)
			workers_.push_back(boost::shared_ptr<worker>(new worker(io_, *connector_, f)));

		int i = 0; for (auto & it : workers_)
		{
			boost::thread t([&] { it->work(); });
			setThreadName(t.get_id(), "launcher " + boost::lexical_cast<std::string>(++i));
		}
	}

	void callback(const std::string & msg)
	{
		// write in the database
		std::cout << "this is a message: " << msg << std::endl;
	}

	boost::asio::io_service io_;
	boost::shared_ptr<connector> connector_;
	std::vector<boost::shared_ptr<worker>> workers_;

	bool finished_ = false;
	boost::mutex m_;
	boost::condition_variable cv_;
};

int main()
{
	service().run();
	return 0;
}