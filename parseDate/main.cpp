#include <tuple>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

struct myDate {

	myDate() {}
	myDate(const int d, const int m, const int y) {
			
	}
};

namespace boost {
namespace spirit {
namespace traits
{
	template<>
	struct transform_attribute<
		::myDate, std::tuple<int, int, int>, qi::domain>
	{
		typedef std::tuple<int, int, int> date_parts;
		typedef date_parts type;

		static date_parts pre(::myDate) {
			return date_parts();
		}

		static void post(::myDate& d, date_parts const& v) {
			d = ::myDate(std::get<0>(v), std::get<1>(v), std::get<2>(v));
		}
	};
}}}

namespace qi = boost::spirit::qi;

int main() {

	std::string test = "10/12/2017";

	qi::rule<
		std::string::const_iterator, 
		myDate(),
		qi::space_type
	> rDate = qi::no_skip[qi::int_ >> '/' >> qi::int_ >> '/' >> qi::int_];

	bool res = qi::phrase_parse(
		  test.cbegin(), test.cend()
		, rDate
		, qi::space);

	return 0;
}