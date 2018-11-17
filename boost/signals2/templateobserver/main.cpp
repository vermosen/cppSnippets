#include "observable.h"
#include "observer.h"

#include <thread>

int main() {
	
	typedef double arg;

	observer<arg> obs;
	observable<arg> o;
	obs.bind(o);

	bool stop = false;
	std::thread t([&]() {
		while (!stop) {
			o.notify((arg)1);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	stop = true;
	t.join();
	return 0;
}