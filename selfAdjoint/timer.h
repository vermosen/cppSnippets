#include <chrono>
#include <iostream>

#define DURATION_T std::chrono::microseconds
#define CLOCK_T std::chrono::high_resolution_clock
#define TIMED_TEST(...)                                 \
DURATION_T mu_s;                                        \
{                                                       \
    std::chrono::time_point<std::chrono::steady_clock>  \
        t = CLOCK_T::now();                             \
    __VA_ARGS__                                         \
    mu_s = std::chrono::duration_cast<DURATION_T>(      \
        CLOCK_T::now() - t);                            \
}

#define TEST_RESULT_MICROSECONDS mu_s.count()
#define TIMED_TEST_MESSAGE()								\
std::cout													\
	<< "test ended after "									\
	<< mu_s.count() << " microseconds"						\
	<< std::endl;								