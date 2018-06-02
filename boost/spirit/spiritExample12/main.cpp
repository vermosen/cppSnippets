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
	std::string name;
	std::string val;
};

BOOST_FUSION_ADAPT_STRUCT(
	node,
	(std::string, name)
	(std::string, val)
)

template <typename iterator>
struct record_parser : qi::grammar<iterator, node(), ascii::space_type>
{
	record_parser() : record_parser::base_type(start)
	{
		using qi::lit;
		using qi::lexeme;
		using qi::raw;
		using ascii::char_;
		using ascii::string;
		using namespace qi::labels;

		using phx::at_c;
		using phx::push_back;

		text = lexeme[+(char_ - '<')[_val += _1]];

		start_tag %=
			'<'
			>> !lit('/')
			>   lexeme[+(char_ - '>')]
			>   '>'
			;

		end_tag =
			"</"
			>   string(_r1)
			>   '>'
			;

		start =
			start_tag[at_c<0>(_val) = _1]
			>> text[at_c<1>(_val) = _1]
			>> end_tag(at_c<0>(_val))
			;
	}

	qi::rule<iterator, node(), ascii::space_type> start;
	qi::rule<iterator, std::string(), ascii::space_type> text;
	qi::rule<iterator, std::string(), ascii::space_type> start_tag;
	qi::rule<iterator, void(std::string), ascii::space_type> end_tag;
};

static const std::string rec("<foo>hello world!</fdsao>");

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
