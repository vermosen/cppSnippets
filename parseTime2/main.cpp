#include <tuple>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

int main() {

	// goal: read a time string in format HH:MM:SS.ffff
	// given a precision and add the corresponding ticks
	std::string test = "10:56:10";

	unsigned int precision = 3;

	boost::fusion::vector<unsigned, unsigned, unsigned, unsigned> args;

	bool res = qi::phrase_parse(
		  test.cbegin(), test.cend()
		, qi::no_skip[qi::uint_[qi::_pass = (qi::_1 < 24)]
			>> ':' >> qi::uint_[qi::_pass = (qi::_1 < 60)]
			>> ':' >> qi::uint_[qi::_pass = (qi::_1 < 60)]
			>> (qi::eps(phx::ref(precision) > 0) >> '.' >> qi::uint_)]
		, qi::space, args);

	return 0;
}