#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>
#include <complex>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

struct node
{
	std::string val1;
	std::string val2;
	int val3;
};

BOOST_FUSION_ADAPT_STRUCT(
	node,
	(std::string, val1)
	(std::string, val2)
	(int, val3)
)

template <typename iterator>
struct record_parser : qi::grammar<iterator, node(), ascii::space_type>
{
	record_parser() : record_parser::base_type(start)
	{
		using qi::lit;
		using qi::lexeme;
		using qi::raw;
		using qi::int_;
		using ascii::char_;
		using ascii::string;
		using namespace qi::labels;

		using phx::at_c;
		using phx::push_back;

		text = lexeme[+(char_ - '<')[_val += _1]];

		_int = lexeme[int_[_val = _1]];

		start_tag =
			'<'
			>> !lit('/')
			>> lexeme[+(char_ - '>')]
			>> '>'
			;

		end_tag =
			"</"
			>> string(_r1)
			>> '>'
			;

		rText =
			start_tag[_a = _1]
			>> text[_val = _1]
			>> end_tag(_a)
			;

		rInt =
			start_tag[_a = _1]
			>> _int[_val = _1]
			>> end_tag(_a)
			;

		start =
			rText[at_c<0>(_val) = _1]
			>> rText[at_c<1>(_val) = _1]
			>> rInt[at_c<2>(_val) = _1];
	}

	qi::rule<iterator, node(), ascii::space_type> start;
	qi::rule<iterator, std::string(), qi::locals<std::string>, ascii::space_type> rText;
	qi::rule<iterator, int(), qi::locals<std::string>, ascii::space_type> rInt;
	qi::rule<iterator, std::string(), ascii::space_type> text;
	qi::rule<iterator, int(), ascii::space_type> _int;
	qi::rule<iterator, std::string(), ascii::space_type> start_tag;
	qi::rule<iterator, void(std::string), ascii::space_type> end_tag;
};

static const std::string rec(
	R"file(
			<a>hello</a>
			<b>world!</b>
			<c>123</c>
		)file");

int main()
{
	typedef std::string::const_iterator iterator_type;
	typedef record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	node emp;
	iterator_type iter = rec.begin(), end = rec.end();

	if (phrase_parse(iter, end, g, ascii::space, emp) && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "got: " << emp.val1 << ", " << emp.val2 << ", " << boost::lexical_cast<int>(emp.val3) << std::endl;
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
