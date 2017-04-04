#include <memory>
#include <array>
#include <utility>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>
#include <boost/regex.hpp>

#include "workerImpl.hpp"
#include "reader.hpp"
#include "connections/http.hpp"
#include "connections/https.hpp"

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
		t_ = std::unique_ptr<boost::thread>(new boost::thread([&] { io_->run(); }));
		setThreadName(t_->get_id(), "io runner");

		cnx_ = boost::shared_ptr<connection>(new https(io_,
			connectionDelegate(boost::bind(&worker::connect_callback, this, _1)), true));
		cnx_->connect("kgc0418-tdw-data-0.s3.amazonaws.com", 80);

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
			write_("host " + cnx_->host() + " successfully reached");

			reader_ = boost::shared_ptr<reader>(new reader(cnx_,
				urlReadDelegate(boost::bind(&worker::reader_callback, this, _1, _2))));
			reader_->getAsync("slices/SLICE_COMMODITIES_2017_04_03_129.zip");
		}
		else // abort or retry ?
		{
			write_("cannot connect to host...");
		}
	}

	void reader_callback(const std::string & msg, bool result)
	{
		if (result)
		{
			counter_++;
			write_("successfully retrieved: " + msg);
			reader_->getAsync("");
		}
		else
		{
			write_("cannot connect to host...");
		}
	}

	// void error_callback() // TODO

	std::unique_ptr<boost::thread> t_;
	boost::condition_variable cv_;
	bool terminate_;
	boost::mutex m_;

	boost::shared_ptr<boost::asio::io_service::work> ioTask_;
	boost::shared_ptr<boost::asio::io_service> io_;
	
	boost::shared_ptr<connection> cnx_;
	boost::shared_ptr<reader> reader_;
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

	void stop()
	{
		//TODO
	}

private:
	std::vector<boost::shared_ptr<worker>> workers_;
	writer w_; int n_;
};
 
int main()
{
	service srv(1);
	srv.start();
	boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
	srv.stop();
	return 0;
}