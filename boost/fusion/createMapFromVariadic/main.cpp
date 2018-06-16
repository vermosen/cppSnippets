#include <string>
#include <iostream>

#include <boost/fusion/container.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>

template <typename ... Args>
struct foo {

	foo(Args ... args) {

		m_map = boost::fusion::make_map<Args...>(args...);
	}

	bool test() {
		return boost::fusion::has_key<std::string>(m_map);
	}

	boost::fusion::map<
		boost::fusion::pair<Args, Args>...
	> m_map;
};

int main() {

	auto m = foo<int, std::string, bool, double>(10, "Boost", true, 3.14);

	if (m.test()) {
		std::cout << boost::fusion::at_key<std::string>(m.m_map) << '\n';
		auto m2 = boost::fusion::erase_key<std::string>(m.m_map);
		auto m3 = boost::fusion::push_back(m.m_map, boost::fusion::make_pair<float>('X'));
		std::cout << std::boolalpha << boost::fusion::has_key<std::string>(m.m_map) << '\n';
	}

	std::cout << boost::fusion::at_key<int>(m.m_map) << '\n';

	return 0;
}