#include <chrono>
#include <atomic>

typedef std::chrono::microseconds duration_type;
typedef std::chrono::high_resolution_clock clock_type;

int main() {

	duration_type dur(10);
	auto test = dur.count();
	test *= 1.3;

	dur = duration_type(test);

	// atomic variant
	std::atomic<duration_type> dur2(duration_type(10));

	return EXIT_SUCCESS;
}
