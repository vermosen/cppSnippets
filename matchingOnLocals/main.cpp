#define BOOST_SPIRIT_DEBUG 

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

int main() {

	std::string test = "-1245+";
	int r;

	qi::rule<std::string::const_iterator, int(), qi::locals<char>, qi::space_type> rulez;

	rulez = qi::omit[qi::char_("-/+")[qi::_a = qi::_1]]
		>> qi::int_[qi::_val = qi::_1]
		>> qi::omit[qi::char_(qi::_a)]
		;

	rulez.name("rulez");
	BOOST_SPIRIT_DEBUG_NODES((rulez))
	debug(rulez);

	bool res = qi::phrase_parse(
		test.cbegin(), test.cend()
		, rulez, qi::space, r);

	return 0;
}