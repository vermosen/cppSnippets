#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>
#include <complex>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct record
{
	int val;
};

BOOST_FUSION_ADAPT_STRUCT(
	record,
	(int, val)
)

template <>
struct boost::spirit::traits::transform_attribute<std::string, int>
{
	typedef int& type;
	static int& pre(int_data& d) { return d.i; }
	static void post(int_data& val, int const& attr) {}
};

template <typename Iterator>
struct record_parser : qi::grammar<Iterator, record(), ascii::space_type>
{
	record_parser() : record_parser::base_type(start)
	{
		using ascii::char_;

		rBase %= qi::lexeme[+(char_ - '>')];
		rInt %= qi::attr_cast(rBase);

		//rStartTag %=
		//	'<'
		//	>> !lit('/')
		//		>   lexeme[+(char_ - '>')]
		//		>   '>'
		//	;

		//rEndTag =
		//	"</"
		//		>   string(_r1)
		//		>   '>'
		//	;

		start %=
			rInt;
	}

	qi::rule<Iterator, std::string(), ascii::space_type> rBase;
	qi::rule<Iterator, int(), ascii::space_type> rInt;
	qi::rule<Iterator, record(), ascii::space_type> start;
};

static const std::string rec("182");

int main()
{
	using boost::spirit::ascii::space;
	typedef std::string::const_iterator iterator_type;
	typedef record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	record emp;
	std::string::const_iterator iter = rec.begin();
	std::string::const_iterator end = rec.end();

	if (phrase_parse(iter, end, g, space, emp) && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "got: " << boost::lexical_cast<std::string>(emp.val) << std::endl;
		std::cout << "\n-------------------------\n";
	}
	else
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing failed\n";
		std::cout << "-------------------------\n";
	}

	system("pause");
	return 0;
}
