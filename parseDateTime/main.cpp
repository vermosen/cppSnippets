#include <tuple>

#define BOOST_SPIRIT_DEBUG 

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/at.hpp>

struct myDate {

	myDate() {}
	myDate(unsigned m, unsigned d, unsigned y, int HH, int MM, int SS, int f, bool local) 
	: m_month(m), m_day(d), m_year(y) {}

	unsigned m_month;
	unsigned m_day;
	unsigned m_year;

	friend std::ostream;
};

std::ostream& operator<<(std::ostream& os, const myDate& d)
{
	os << "[" << d.m_year << "-" << d.m_month << "-" << d.m_day << "]";
	return os;
}

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace boost {
namespace spirit {
namespace traits {
	template<>
	struct transform_attribute<
		myDate, boost::fusion::vector7<unsigned, unsigned, 
			unsigned, unsigned, unsigned, unsigned, unsigned>, qi::domain> {

		typedef boost::fusion::vector7<unsigned, unsigned,
			unsigned, unsigned, unsigned, unsigned, unsigned> type;
		typedef myDate return_type;

		static type pre(return_type& d) { return type(); }
		static void post(return_type& val, const type& v) {

			val = myDate(
				  boost::fusion::at_c<0>(v)
				, boost::fusion::at_c<1>(v)
				, boost::fusion::at_c<2>(v)
				, boost::fusion::at_c<3>(v)
				, boost::fusion::at_c<4>(v)
				, boost::fusion::at_c<5>(v)
				, boost::fusion::at_c<6>(v)
			    , true);				// todo: pass as an addition member
		}
		static void fail(return_type&) {}
	};
}}}

int main() {

	std::string test = "10/12/2017 12:12:12.123456";

	myDate d;
	bool local = true;
	qi::rule<std::string::const_iterator
		, myDate(bool), qi::space_type> rule;

	rule = qi::attr_cast(
			qi::uint_[qi::_pass = (qi::_1 < 13 && qi::_1 > 0)] >> '/'
				>> qi::uint_[qi::_pass = (qi::_1 < 32 && qi::_1 > 1)] >> '/'
				>> qi::uint_[qi::_pass = (qi::_1 > 1970)]
				>> qi::uint_[qi::_pass = (qi::_1 < 24 && qi::_1 >= 0)] >> ':'
				>> qi::uint_[qi::_pass = (qi::_1 < 60 && qi::_1 >= 0)] >> ':'
				>> qi::uint_[qi::_pass = (qi::_1 < 60 && qi::_1 >= 0)] >> '.'
				>> qi::uint_
				);

	rule.name("rule");
	BOOST_SPIRIT_DEBUG_NODES((rule))
	debug(rule);

	bool res = qi::phrase_parse(
		  test.cbegin(), test.cend()
		, rule(phx::ref(local)), qi::space, d);

	return 0;
}