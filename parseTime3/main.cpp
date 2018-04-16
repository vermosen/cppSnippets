#include <tuple>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/at.hpp>

#define memchr_in_long_block(s,c,l) memchrSSE2(s,c,l)

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace boost { namespace spirit { namespace traits {
	template <>
	struct transform_attribute<unsigned, std::vector<char>, qi::domain> {
		typedef std::vector<char> type;
		static type pre(unsigned& d) { return {}; }
		static void post(unsigned& val, const type& attr) {
			for (std::size_t i = 0; i < attr.size(); i++)
				val = 10 * val + attr[i] - '0';
		}
		static void fail(unsigned&) {}
	};
}}}

int main() {

	// goal: read a time string in format HH:MM:SS.ffff
	// given a precision and add the corresponding ticks
	std::string test = "10:56:10.12344";

	std::size_t precision = 3;
	std::size_t sz = test.size();
	std::size_t eff = std::min(sz - 9, precision);

	boost::fusion::vector<unsigned, unsigned, unsigned, unsigned> args;

	bool res = qi::phrase_parse(
		test.cbegin(), test.cend()
		, qi::no_skip[qi::uint_[qi::_pass = (qi::_1 < 24)]
		>> ':' >> qi::uint_[qi::_pass = (qi::_1 < 60)]
		>> ':' >> qi::uint_[qi::_pass = (qi::_1 < 60)]
		>> (qi::eps(phx::ref(sz) > 8) >> '.' >>
			qi::attr_cast(qi::repeat(phx::ref(eff))[qi::char_]))]
		, qi::space, args);

	if (eff < precision) {
		boost::fusion::at_c<3>(args) *= std::pow(10, precision - eff);
	}
	else {
		boost::fusion::at_c<3>(args) /= std::pow(10, eff - precision);
	}
	
	std::cout << boost::fusion::at_c<3>(args) << std::endl;
	return 0;
}

/*
#include <tuple>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

int main() {

// goal: read a time string in format HH:MM:SS.ffff
// given a precision and add the corresponding ticks
std::string test = "10:56:10.12344";

unsigned int precision = 3;

boost::fusion::vector<unsigned, unsigned, unsigned, std::string> args;

bool res = qi::phrase_parse(
	test.cbegin(), test.cend()
	, qi::no_skip[qi::uint_[qi::_pass = (qi::_1 < 24)]
	>> ':' >> qi::uint_[qi::_pass = (qi::_1 < 60)]
	>> ':' >> qi::uint_[qi::_pass = (qi::_1 < 60)]
	>> (qi::eps(phx::ref(precision) > 0) >> '.' >> qi::as_string[qi::repeat(4)[qi::char_]])]
	, qi::space, args);

return 0;
}
*/