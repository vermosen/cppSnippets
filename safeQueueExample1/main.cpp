// condition_variable example
#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <iostream>								// std::cout
#include <boost/thread/thread.hpp>				// std::thread
#include <boost/thread/mutex.hpp>				// std::mutex, std::unique_lock
#include <boost/thread/condition_variable.hpp>	// std::condition_variable
#include <boost/date_time/posix_time/posix_time.hpp>
#include <queue>

class safeQueue
{
public:
	void enqueue(char c)
	{
		boost::lock_guard<boost::mutex> lock(m);
		q.push(c);
		cond.notify_one();
	}

	char dequeue()
	{
		boost::unique_lock<boost::mutex> lock(m);
		while (q.empty())
		{
			cond.wait(lock);		// release lock as long as the wait and reaquire it afterwards.
		}

		char val = q.front();
		q.pop();
		return val;
	}

private:
	std::queue<char> q;
	mutable boost::mutex m;
	boost::condition_variable cond;
};

safeQueue queue;
const std::string str("hello World !");

void write()
{
	for (auto c : str)
	{
		queue.enqueue(c);
		boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
	}

	queue.enqueue('\0');
}

void read()
{
	for (;;)
	{
		char c = queue.dequeue();
		std::cout << c;

		if (c == '\0') return;
	}
}

int main()
{
	boost::thread threads[2];

	// spawn 10 threads:
	threads[0] = boost::thread(write);
	threads[1] = boost::thread(read);

	for (auto& th : threads) th.join();

	return 0;
}

#endif
