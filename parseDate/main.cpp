#include <tuple>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

struct myDate {

	myDate() {}
	myDate(unsigned m, unsigned d, unsigned y) 
	: m_month(m), m_day(d), m_year(y) {}

	unsigned m_month;
	unsigned m_day;
	unsigned m_year;

};

namespace qi = boost::spirit::qi;

namespace boost {
namespace spirit {
namespace traits {
	template<>
	struct transform_attribute<
		myDate, boost::fusion::vector3<unsigned, unsigned, unsigned>, qi::domain> {
		typedef boost::fusion::vector3<unsigned, unsigned, unsigned> type;
		static type pre(myDate& d) { return type(); }
		static void post(myDate& val, const type& v) {

			unsigned m = boost::fusion::at_c<0>(v);
			unsigned d = boost::fusion::at_c<1>(v);
			unsigned y = boost::fusion::at_c<2>(v);

			val = myDate(m, d, y);
		}
		static void fail(myDate&) {}
	};
}}}

int main() {

	std::string test = "10/12/2017";

	myDate d;

	bool res = qi::phrase_parse(
		  test.cbegin(), test.cend()
		, qi::attr_cast(
			qi::no_skip[qi::uint_ >> '/' 
				>> qi::uint_ >> '/' 
				>> qi::uint_
			]
		), qi::space, d);

	return 0;
}