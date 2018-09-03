#pragma warning(disable : 4996)

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <sstream>

std::timed_mutex m_mutex;
bool success;

std::string now() {
	std::stringstream ss;
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	ss << std::put_time(std::localtime(&now_c), "%F %T");
	return ss.str();
}

void init() {
	if (m_mutex.try_lock_for(std::chrono::seconds(15))) {
		std::cout << "[init] locking mutex at " << now() << std::endl;
		std::cout << "[init] working..." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(10));
		success = true;
	}
	else {
		std::cout << "unable to aquire mutex" << std::endl;
		success = false;
	}

	std::cout << "[init] finished, exit!" << std::endl;
	m_mutex.unlock();
}

void run() {
	std::cout << "[run] arrived at " << now() << std::endl;
	std::lock_guard<std::timed_mutex> lock(m_mutex);

	if (success) {
		std::cout << "[run] it is " << now() << std::endl;
		std::cout << "[run] working..." << std::endl;
	}

	std::cout << "[init] finished, exit!" << std::endl;
}

int main()
{
	std::thread th1(init);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::thread th2(run);

	th1.join();
	th2.join();

	return 0;
}