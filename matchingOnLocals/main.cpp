#define BOOST_SPIRIT_DEBUG 

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

int main() {

	typedef boost::fusion::vector<char, int, char> retval;

	std::string test = "-1245-";
	retval r;

	qi::rule<std::string::const_iterator, retval(), qi::space_type, qi::locals<char>> rulez;

	rulez = qi::char_("-")[qi::_val = qi::_a]
		>> qi::int_
		>> qi::char_(qi::_a)
		;

	rulez.name("rulez");
	BOOST_SPIRIT_DEBUG_NODES((rulez))
	debug(rulez);

	bool res = qi::phrase_parse(
		test.cbegin(), test.cend()
		, rulez, qi::space, r);

	return 0;
}