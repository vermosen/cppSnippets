#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/chrono.hpp>

#include <iostream>
#include <string>
#include <complex>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

///////////////////////////////////////////////////////////////////////////
//  Our employee struct
///////////////////////////////////////////////////////////////////////////
struct record
{
	std::string val1;
};

BOOST_FUSION_ADAPT_STRUCT(
	record,
	(std::string, val1)
)

template <typename iterator, typename skipper>
struct parser : qi::grammar<iterator, record(), skipper>
{
	parser() : parser::base_type(rFile)
	{
		rBase
			%= qi::lexeme[+(qi::char_ - '<')];

		//rStart
		//	%= '<' >> !qi::lit('/') > qi::lexeme[+(qi::char_ - '>')] > '>';

		//rEnd
		//	%= "</" > ascii::string(qi::_r1) > '>';

		//rString
		//	%= rStart[qi::_a = qi::_1] > rBase > rEnd(qi::_a);

		rStart
			%= '<' >> +(qi::char_ - '>') > '>';

		rEnd
			%= "</" >> +(qi::char_) > '>';

		rString
			%= rStart >> qi::lexeme[rBase] >> rEnd;

		rFile
			%= rString >> qi::eoi;
	}

	qi::rule<iterator, std::string(), skipper> rBase;
	//qi::rule<iterator, std::string(), skipper> rStart;
	//qi::rule<iterator, void(std::string), skipper> rEnd;
	qi::rule<iterator, void(), skipper> rStart;
	qi::rule<iterator, void(), skipper> rEnd;
	qi::rule<iterator, std::string(), skipper> rString;
	qi::rule<iterator, record(), skipper> rFile;
};

static const std::string rec("<test>hello World!</test>");

int main()
{
	typedef std::string::const_iterator iterator_type;

	parser<iterator_type, ascii::space_type> g; // Our grammar

	record emp;
	std::string::const_iterator iter = rec.begin();
	std::string::const_iterator end = rec.end();

	if (phrase_parse(iter, end, g, ascii::space, emp) && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "got: " << emp.val1 << std::endl;
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
