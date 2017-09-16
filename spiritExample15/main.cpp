#define BOOST_SPIRIT_DEBUG

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

struct child
{
	std::string val1;
	int val2;
};

struct settings
{
	std::string val1;
	std::vector<child> val2;
	int val3;
};

BOOST_FUSION_ADAPT_STRUCT(
	child,
	(std::string, val1)
	(int, val2)
)

BOOST_FUSION_ADAPT_STRUCT(
	settings,
	(std::string, val1)
	(std::vector<child>, val2)
	(int, val3)
)

template <typename iterator>
struct record_parser : qi::grammar<iterator, settings(), ascii::space_type>
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

		rTextBase =
			lexeme[+(char_ - '<')[_val += _1]]
			;

		rIntBase =
			lexeme[int_[_val = _1]]
			;

		rHeader %=
			"<?"
			>> !lit('/')
			>> lexeme[+(char_ - "?>")]
			>> "?>"
			;

		rStartTag =
			'<'
			>> !lit('/')
			>> qi::lit(_r1)[_val = _r1]
			>> '>'
			;

		rEndTag =
			"</"
			>> string(_r1)
			>> '>'
			;

		rText =
			rStartTag(_r1)[_a = _1]
			>> rTextBase[_val = _1]
			>> rEndTag(_a)
			;

		rInt =
			rStartTag(_r1)[_a = _1]
			>> rIntBase[_val = _1]
			>> rEndTag(_a)
			;

		rChild =
			rStartTag(std::string("child"))[_a = _1]
			>> rText(std::string("val1"))[at_c<0>(_val) = _1]
			>> rInt(std::string("val2"))[at_c<1>(_val) = _1]
			>> rEndTag(_a)
			;

		rChildren =
			*(rChild)
			;

		rSettings =
			rStartTag(std::string("settings"))[_a = _1]
			>> rText(std::string("val1"))[at_c<0>(_val) = _1]
			>> rChildren[at_c<1>(_val) = _1]
			>> rInt(std::string("val3"))[at_c<2>(_val) = _1]
			>> rEndTag(_a)
			;

		start =
			rHeader
			>> rSettings[_val = _1]
			;

		rHeader.name("rHeader");
		BOOST_SPIRIT_DEBUG_NODE(rHeader);

		rChild.name("rChild");
		BOOST_SPIRIT_DEBUG_NODE(rChild);

		rChildren.name("rChildren");
		BOOST_SPIRIT_DEBUG_NODE(rChildren);

		rSettings.name("rSettings");
		BOOST_SPIRIT_DEBUG_NODE(rSettings);

		start.name("start");
		BOOST_SPIRIT_DEBUG_NODE(start);
	}

	qi::rule<iterator, settings(), qi::locals<std::string>, ascii::space_type> rSettings;
	qi::rule<iterator, child(), qi::locals<std::string>, ascii::space_type> rChild;
	qi::rule<iterator, std::vector<child>(), ascii::space_type> rChildren;
	qi::rule<iterator, settings(), ascii::space_type> start;
	qi::rule<iterator, std::string(std::string), qi::locals<std::string>, ascii::space_type> rText;
	qi::rule<iterator, int(std::string), qi::locals<std::string>, ascii::space_type> rInt;
	qi::rule<iterator, std::string(), ascii::space_type> rTextBase;
	qi::rule<iterator, int(), ascii::space_type> rIntBase;
	qi::rule<iterator, std::string(std::string), ascii::space_type> rStartTag;
	qi::rule<iterator, void(std::string), ascii::space_type> rEndTag;
	qi::rule<iterator, std::string(), ascii::space_type> rHeader;
};

//<?xml version="1.0" encoding="utf-8"?>
static const std::string rec( 
	R"file(
			<?xml version="1.0" encoding="utf-8"?>
			<settings>
				<val1>foo</val1>
				<child>
					<val1>hello</val1>
					<val2>1</val2>
				</child>
				<child>
					<val1>world!</val1>
					<val2>1</val2>
				</child>
				<val3>2</val3>
			</settings>
		)file");

int main()
{
	typedef std::string::const_iterator iterator_type;
	typedef record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	settings emp;
	iterator_type iter = rec.begin(), end = rec.end();

	if (phrase_parse(iter, end, g, ascii::space, emp) && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
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