#include <iostream>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>

typedef boost::mpl::vector<int, double>::type sequence;

int main () {
	static_assert(boost::mpl::size<sequence>::value != 1, "more than 1 item!");
}

