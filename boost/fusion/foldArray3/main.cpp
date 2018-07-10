#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/container.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/size.hpp>

struct printer {

	printer() : first(true) {}

	typedef std::ostream& result_type;

	template<typename T>
	std::ostream& operator()(std::ostream& os, const T& t) const
	{
		if (!first) {
			os << " AND ";
		}

		os << t.second << "=" << t.first;

		first = false;
		return os;
	}

	mutable bool first;
};

int main() {

	typedef boost::fusion::vector<int, int, double, bool> data_type;
	typedef boost::fusion::vector<const char*, const char*, const char*, const char*> fields_type;
	fields_type flds = { "id", "foo", "faa", "baz" };


	// result structure we are looking for
	typedef boost::fusion::vector<
		  std::pair<int		, const char *>
		, std::pair<int		, const char *>
		, std::pair<double	, const char *>
		, std::pair<bool	, const char *>
	> map_type;

	auto mm = map_type(
		  std::make_pair(10, "id")
		, std::make_pair(10, "foo")
		, std::make_pair(3.1415, "faa")
		, std::make_pair(true, "baz"));

	boost::fusion::fold(mm, std::cout, printer());
	std::cout << std::endl;
	return 0;
}
