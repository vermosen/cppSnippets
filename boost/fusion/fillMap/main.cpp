#include <string>
#include <iostream>

#include <boost/fusion/container.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>

int main()
{

	boost::fusion::map<
		  boost::fusion::pair<int, int>
		, boost::fusion::pair<std::string, std::string>
		, boost::fusion::pair<bool, bool>
		, boost::fusion::pair<double, double>
	> m = boost::fusion::make_map<int, std::string, bool, double>(10, "Boost", true, 3.14);

	if (boost::fusion::has_key<std::string>(m)) {
		std::cout << boost::fusion::at_key<std::string>(m) << '\n';
		auto m2 = boost::fusion::erase_key<std::string>(m);
		auto m3 = boost::fusion::push_back(m2, boost::fusion::make_pair<float>('X'));
		std::cout << std::boolalpha << boost::fusion::has_key<std::string>(m3) << '\n';
	}

	std::cout << boost::fusion::at_key<int>(m) << std::endl;

	return 0;
}