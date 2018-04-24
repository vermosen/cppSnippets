#include <map>
#include <vector>
#include <string>

#define BOOST_SPIRIT_DEBUG 

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/std_pair.hpp> 
#include <boost/fusion/include/adapt_struct.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;


struct datetime {

	datetime() {}
	datetime(unsigned int y, unsigned int m, unsigned int d) 
	{
		m_y = y;
		m_m = m;
		m_d = d;
	}

	inline bool operator< (const datetime rhs) const  {
		return m_d < rhs.m_d;
	}
	friend std::ostream;

	unsigned int m_y;
	unsigned int m_m;
	unsigned int m_d;
};

std::ostream& operator<<(std::ostream& os, const datetime& d) {
	os << "[ date ]";
	return os;
}

typedef std::map<datetime, double> container_type;

struct myStruct {
	std::string m_id;
	std::map<datetime, double> m_map;
};

BOOST_FUSION_ADAPT_STRUCT(
	  myStruct, 
	(std::string, m_id)
	(container_type, m_map)
)

template <typename Iterator>
struct keys_and_values
	: qi::grammar<Iterator, myStruct(), qi::space_type> {

	keys_and_values() : keys_and_values::base_type(rStart) {

		rDate =
			(
				   qi::ushort_ >> '-'
				>> qi::ushort_ >> '-'
				>> qi::ushort_
			)
			[
				qi::_val = phx::construct<datetime>(
					qi::_1, qi::_2, qi::_3)
			];

		rValue = qi::double_;
		rPair = '(' >> rDate >> ',' >> qi::double_ >> ')';
		rMap = *(*qi::lit(' ') >> rPair);

		rStart = qi::lexeme[*qi::char_("A-Z")]
			>> rMap;

		rStart.name("rStart");
		BOOST_SPIRIT_DEBUG_NODES((rStart))
		debug(rStart);

		rPair.name("rPair");
		BOOST_SPIRIT_DEBUG_NODES((rPair))
		debug(rPair);

		rValue.name("rValue");
		BOOST_SPIRIT_DEBUG_NODES((rValue))
		debug(rValue);
	}

	qi::rule<Iterator, myStruct(), qi::space_type> rStart;
	qi::rule<Iterator, std::map<datetime, double>()> rMap;
	qi::rule<Iterator, std::pair<datetime, double>()> rPair;
	qi::rule<Iterator, double()> rValue;
	qi::rule<Iterator, datetime()> rDate;
};

int main() {

	std::string test = "SPY (2010-04-23,2.35) (2010-04-24,2.35)(2010-04-25,2.35) (2010-04-27,2.35) (2010-04-28,2.35)";
	keys_and_values<std::string::iterator> parser;
	myStruct my_struct;

	bool res = qi::phrase_parse(
		test.begin(), test.end()
		, parser, qi::space
		, my_struct);

	return 0;
}