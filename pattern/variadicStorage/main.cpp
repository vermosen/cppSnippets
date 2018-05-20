
#include <tuple>

template <typename ... Args>
struct foo {

	foo(Args ... args) : m_args(std::forward<Args>(args)...) {}
	std::tuple<Args...> m_args;
};


int main() {

	foo<int, int, double> f(3, 3, 1.0);
	return 0;
}