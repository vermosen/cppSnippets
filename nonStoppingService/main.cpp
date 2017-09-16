#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/utility/in_place_factory.hpp>

std::atomic<bool> stop;

class printer
{
public:
	printer() {}
	void print(const std::string & msg)
	{
		boost::mutex::scoped_lock lock(m_);
		std::cout << msg << std::endl;
	}

private:
	boost::mutex m_;
};

class worker
{
public:
	worker(boost::shared_ptr<boost::asio::io_service> io,
		boost::function<void(const std::string &)> callback,
		int id)
		: strand_(*io)
		, id_(id)
		, callback_(callback)
		, timer_(*io, boost::posix_time::seconds(1)),
		count_(0) {}

	~worker()
	{
		callback_("Final count is " + boost::lexical_cast<std::string>(count_));
	}

	void work()
	{
		while(!stop)
		{
			callback_("Timer " + boost::lexical_cast<std::string>(id_) + ": " +
				boost::lexical_cast<std::string>(count_));

			++count_;

			timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
			timer_.async_wait(strand_.wrap(boost::bind(&worker::work, this)));
		}
	}

private:
	int id_;
	boost::asio::strand strand_;
	boost::asio::deadline_timer timer_;
	boost::function<void(const std::string &)> callback_;
	int count_;
};

int main()
{
	stop = false;

	printer p;

	// initialize and run the service
	boost::shared_ptr<boost::asio::io_service> io(
		new boost::asio::io_service);

	boost::scoped_ptr<boost::asio::io_service::work> work(
		new boost::asio::io_service::work(*io));

	// runs in a separate thread
	boost::thread t([&] { io->run(); });

	// now we add the jobs in separate threads
	int size = 5; std::vector<boost::thread> workers;

	auto f = boost::function<void(const std::string &)>(boost::bind(&printer::print, &p, _1));

	for (int i = 0; i < size; i++)
	{
		boost::shared_ptr<worker> w(new worker(io, f, i));
		workers.push_back(boost::thread(boost::bind(&worker::work, w)));
	}

	boost::this_thread::sleep(boost::posix_time::seconds(3));
	stop = true;

	for (auto & i : workers)
	{
		i.join();
	}

	work.reset();

	t.join();

	return 0;
}