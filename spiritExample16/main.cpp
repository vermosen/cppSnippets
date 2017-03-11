#define BOOST_SPIRIT_DEBUG

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>

#include <iostream>
#include <string>
#include <complex>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

struct settings
{
	boost::gregorian::date startDate;
	boost::gregorian::date endDate;
};

BOOST_FUSION_ADAPT_STRUCT(
	settings,
	(boost::gregorian::date, startDate)
	(boost::gregorian::date, endDate)
)

typedef boost::tuple<int, int, int> dateAdaptor;

template<>
struct boost::spirit::traits::transform_attribute<boost::gregorian::date, dateAdaptor, qi::domain>
{
	typedef dateAdaptor type;

	static type pre(boost::gregorian::date a) { return type(); }

	static void post(boost::gregorian::date& d, type const& v)
	{
		d = boost::gregorian::date(boost::get<0>(v), boost::get<1>(v), boost::get<2>(v));
	}

	static void fail(boost::gregorian::date&) {}
};

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
			*(qi::char_ - "</")
			;

		rDateBase =
			qi::int_[_pass = (qi::_1 >= 1400 && qi::_1 < 10000)][at_c<0>(_val) = _1]
			> '-'
			> qi::int_[_pass = (qi::_1 > 0 && qi::_1 <= 12)][at_c<1>(_val) = _1]
			> '-'
			> qi::int_[_pass = (qi::_1 > 0 && qi::_1 <= 31)][at_c<2>(_val) = _1]
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
			> rTextBase[_val = _1]
			> rEndTag(_a)
			;

		rDate =
			rStartTag(_r1)[_a = _1]
			> rDateBase[_val = _1]
			> rEndTag(_a)
			;

		start =
			rDate(std::string("startDate")) >>
			rDate(std::string("endDate"))
			;

		start.name("start");
		BOOST_SPIRIT_DEBUG_NODE(start);

		rDate.name("rDate");
		BOOST_SPIRIT_DEBUG_NODE(rDate);

		rDateBase.name("rDateBase");
		BOOST_SPIRIT_DEBUG_NODE(rDateBase);
	}

	qi::rule<iterator, settings(), ascii::space_type> start;

	qi::rule<iterator, dateAdaptor(std::string), qi::locals<std::string>, ascii::space_type> rDate;
	qi::rule<iterator, dateAdaptor(), ascii::space_type> rDateBase;
	qi::rule<iterator, std::string(std::string), qi::locals<std::string>, ascii::space_type> rText;
	qi::rule<iterator, std::string(), ascii::space_type> rTextBase;

	qi::rule<iterator, std::string(std::string), ascii::space_type> rStartTag;
	qi::rule<iterator, void(std::string), ascii::space_type> rEndTag;
};

static const std::string rec(
	R"file(
			<startDate>2017-11-01</startDate>
			<endDate>2017-01-11</endDate>
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