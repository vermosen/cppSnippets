#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>

class worker
{
public:
	worker(	boost::shared_ptr<boost::asio::io_service> io, 
			boost::function<void(const std::string &)> callback, 
			int id)
		: strand_(*io)
		, id_(id)
		, callback_(callback)
		, timer_(*io, boost::posix_time::seconds(1)),
		count_(0)
	{
		timer_.async_wait(strand_.wrap(boost::bind(&worker::print, this)));
	}

	~worker()
	{
		callback_("Final count is " + boost::lexical_cast<std::string>(count_));
	}

	void print()
	{
		if (count_ < 5)
		{
			callback_("Timer " + boost::lexical_cast<std::string>(id_) + ": " +
				boost::lexical_cast<std::string>(count_));

			++count_;

			timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
			timer_.async_wait(strand_.wrap(boost::bind(&worker::print, this)));
		}
	}

private:
	int id_;
	boost::asio::strand strand_;
	boost::asio::deadline_timer timer_;
	boost::function<void(const std::string &)> callback_;
	int count_;
};

boost::mutex m;

void print(const std::string & msg)
{
	boost::mutex::scoped_lock lock(m);
	std::cout << msg << std::endl;
}

int main()
{
	boost::shared_ptr<boost::asio::io_service> io(new boost::asio::io_service);

	int size = 20;
	std::vector<boost::shared_ptr<worker>> workers;
	auto f = boost::function<void(const std::string &)>(print);

	for (int i = 0; i < size; i++)
	{
		workers.push_back(boost::shared_ptr<worker>(
			new worker(io, f, i)));
	}

	boost::thread t(boost::bind(&boost::asio::io_service::run, io));
	io->run(); t.join();

	// call worker destructor
	for (int i = 0; i < size; i++)
	{
		workers[i].reset();
	}

	return 0;
}