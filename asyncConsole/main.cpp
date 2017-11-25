#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>
#include <boost/scoped_ptr.hpp>

bool finished = false;
boost::mutex m;
boost::condition_variable cv;

void exit()
{
	boost::this_thread::sleep_for(boost::chrono::seconds(5));
	boost::unique_lock<boost::mutex> lk(m);
	finished = true;
	cv.notify_one();
}

// multithreaded version of urlReader1
int main()
{
	// runs in a separate thread
	boost::thread t([&] { exit(); });

	// exit
	boost::unique_lock<boost::mutex> lk(m);

	while (!finished) cv.wait(lk);

	return 0;
}