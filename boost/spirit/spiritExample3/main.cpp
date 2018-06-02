#include <string>
#include <tuple>

#include <boost/config/warning_disable.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/optional.hpp>
#include <boost/none.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/phoenix/bind.hpp>

using namespace boost::spirit;

#define OPT
#ifdef OPT
typedef boost::optional<boost::gregorian::date> date;
typedef boost::optional<std::tuple<int, int, int> > dateAdaptator;
#else
typedef boost::gregorian::date date;
typedef std::tuple<int, int, int> dateAdaptator;
#endif

struct record
{
	int elem1;
	date elem2;
};

BOOST_FUSION_ADAPT_STRUCT(
	record,
	(int, elem1)
	(date, elem2)
)

template<>
struct traits::transform_attribute<date, dateAdaptator, qi::domain>
{
	typedef dateAdaptator type;

	static type pre(date a) { return type(); }

	static void post(date& d, type const& v)
	{
		#ifdef OPT
		(v ? d = boost::gregorian::date(std::get<0>(*v), std::get<1>(*v), std::get<2>(*v)) : d = boost::none);
		#else
		d = date(std::get<0>(v), std::get<1>(v), std::get<2>(v));	
		#endif
	}

	static void fail(date&) {}
};

template <typename Iterator>
struct record_parser : qi::grammar<Iterator, record(), ascii::space_type>
{

	record_parser() : record_parser::base_type(start)
	{
		quoted_int %= qi::lexeme['"' >> int_ >> '"'];

		#ifdef OPT
		quoteddate %= ascii::no_case["\"\""] | qi::lexeme['"' >> qi::int_ >> "-" >> qi::int_ >> "-" >> qi::int_ >> '"'];
		#else
		quoteddate %= qi::lexeme['"' >> qi::int_ >> "-" >> qi::int_ >> "-" >> qi::int_ >> '"'];
		#endif
		
		start %=
			quoted_int >> ',' >>
			quoteddate
			;
	}
	
	qi::rule<Iterator, int(), ascii::space_type> quoted_int;
	qi::rule<Iterator, dateAdaptator(), ascii::space_type> quoteddate;
	qi::rule<Iterator, record(), ascii::space_type> start;
};

#ifdef OPT
static const std::string rec("\"123456\",\"\"");
#else
static const std::string rec("\"123456\",\"2016-10-12\"");
#endif

int main()
{
	typedef std::string::const_iterator iterator_type;
	typedef record_parser<iterator_type> record_parser;

	record_parser g;
	record result;
	iterator_type iter = rec.begin();
	iterator_type end = rec.end();

	if (phrase_parse(iter, end, g, ascii::space, result) && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "got: " << result.elem1 << ",";
		#ifdef OPT
		if (!result.elem2) 
			{ std::cout << "(none)" << std::endl; } 
		else 
			{ std::cout << result.elem2 << std::endl; }
		#else
		std::cout << result.elem2 << std::endl;
		#endif
		std::cout << "\n-------------------------\n";
	}
	else
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing failed\n";
		std::cout << "-------------------------\n";
	}

	return 0;
}
