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

	typedef std::string result_type;

	template<typename T>
	std::string operator()(const std::string& str, const T& t) const
	{
		std::string res = t.second + std::string("=") + boost::lexical_cast<std::string>(t.first);

		if (!first) {
			res += " AND " + str;
		}

		first = false;
		return res;
	}

	mutable bool first;
};

int main() {

	typedef boost::fusion::vector<int, int, double, bool> data_type;
	typedef std::array<std::string, 4> fields_type;

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

	auto res = boost::fusion::fold(mm, std::string(""), printer());

	std::cout << res << std::endl;
	return 0;
}
