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

typedef std::map<std::string, double> container_type;

struct myStruct {
	std::string m_id;
	std::map<std::string, double> m_map;
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
		rValue = qi::double_;
		rPair = '{' >> qi::char_ >> ',' >> qi::double_ >> '}';
		rMap = *(rPair >> ',') >> rPair;

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
	qi::rule<Iterator, std::map<std::string, double>()> rMap;
	qi::rule<Iterator, std::pair<std::string, double>()> rPair;
	qi::rule<Iterator, double()> rValue;
};

int main() {

	std::string test = "SPY {2,2.35},{3,2.35}";
	keys_and_values<std::string::iterator> parser;
	myStruct my_struct;

	bool res = qi::phrase_parse(
		test.begin(), test.end()
		, parser, qi::space
		, my_struct);

	return 0;
}