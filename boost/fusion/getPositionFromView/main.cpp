#include <string>
#include <iostream>

#include <boost/fusion/container.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>

int main() {

	auto m = boost::fusion::vector<int, double, bool>(10, 3.1415, true);



	return 0;
}
