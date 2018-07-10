#include <boost/fusion/container/vector.hpp>

#include <string>
#include <iostream>
#include <typeinfo>

namespace fusion = boost::fusion;

typedef fusion::vector<int, double, bool> vec_type;

template <typename ... Args>
vec_type fill(Args ... args) {

	return vec_type(args...);
}

int main(int argc, char** argv) {

	auto ret = fill(3, 3.0, true);

	return 0;
}
