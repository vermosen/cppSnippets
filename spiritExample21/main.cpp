#define BOOST_SPIRIT_DEBUG

#include <boost/chrono.hpp>
#include <boost/optional.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/phoenix/bind.hpp>

#include <iostream>
#include <string>
#include <complex>

namespace test
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	///////////////////////////////////////////////////////////////////////////
	//  Our employee struct
	///////////////////////////////////////////////////////////////////////////
	struct record
	{
		std::string surname;
		std::string forename;
		boost::optional<double> salary;
	};
	//]
}

//[tutorial_employee_adapt_struct
BOOST_FUSION_ADAPT_STRUCT(
	test::record,
	(std::string, surname)
	(std::string, forename)
	(boost::optional<double>, salary)
)
//]

using namespace boost::spirit;

struct errorHandler
{
	errorHandler() = default;
	errorHandler(errorHandler const&) = delete;

	template<typename...> struct result { typedef void type; };
	template<typename Iter> void operator()(
		Iter & first_iter, Iter last_iter,
		Iter error_iter, const qi::info& what) const
	{
		Iter start = error_iter, end = start;

		// select the record
		while (!(*end == '"' && *(end + 1) == ',' && *(end + 2) == '"'))
		{
			end++;
		};

		if (start == end) return;

		if (!isascii(*start))
		{
			for (auto it = start; it != (end + 1); ++it)
			{
				if (!isascii(*it)) *it = (char)0x20;
			}
		}
		else
		{
			throw std::exception();
		}
	}
};

namespace test
{
	///////////////////////////////////////////////////////////////////////////////
	//  Our employee parser
	///////////////////////////////////////////////////////////////////////////////
	//[tutorial_employee_parser
	template <typename Iterator>
	struct record_parser : qi::grammar<Iterator, std::vector<test::record>(), ascii::blank_type>
	{
		record_parser() : record_parser::base_type(rRecords)
		{
			using qi::int_;
			using qi::lit;
			using qi::double_;
			using qi::lexeme;
			using ascii::char_;
			using qi::eps;

			rString
				%= lexeme[eps > *(char_ - lit("\",\""))];

			rOptDouble
				%= -(double_);

			rRecord 
				%= lit("record")
				> "{\""
				> rString		> "\",\""
				> rString		> "\",\""
				> rOptDouble	> "\"}"
				;

			rRecords = +(rRecord >> qi::eol);

			qi::on_error<qi::retry>
			(
				rRecord, boost::phoenix::bind
				(boost::phoenix::ref(errorHandler_),
					qi::_1, // it start
					qi::_2, // it end
					qi::_3, // it error
					qi::_4  // error what 
				)
			);

			rString.name("rString");
			BOOST_SPIRIT_DEBUG_NODES((rString))
			debug(rString);
		}

		qi::rule<Iterator, std::string(), ascii::blank_type> rString;
		qi::rule<Iterator, boost::optional<double>(), ascii::blank_type> rOptDouble;
		qi::rule<Iterator, record(), ascii::blank_type> rRecord;
		qi::rule<Iterator, std::vector<record>(), ascii::blank_type> rRecords;

		errorHandler errorHandler_;
	};
	//]
}

static std::string recs("record{\"verm€osen\",\"\",\"160000\"}\n");
boost::chrono::high_resolution_clock::time_point start;

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////

int main()
{
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "\t\tAn employee parser for Spirit...\n\n";
	std::cout << "/////////////////////////////////////////////////////////\n\n";

	using boost::spirit::ascii::blank;
	typedef std::string::iterator iterator_type;
	typedef test::record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	start = boost::chrono::high_resolution_clock::now();

	std::vector<test::record> emps;
	iterator_type iter = recs.begin();
	iterator_type end = recs.end();

	bool r = boost::spirit::qi::phrase_parse(iter, end, g, blank, emps);

	if (r)
	{
		std::cout << boost::fusion::tuple_open('[');
		std::cout << boost::fusion::tuple_close(']');
		std::cout << boost::fusion::tuple_delimiter(", ");

		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "got: " << emps.size() << " employees" << std::endl;
		std::cout << "\n-------------------------\n";
	}
	else
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing failed\n";
		std::cout << "-------------------------\n";
	}

	std::cout << "Conversion done in " << boost::chrono::duration_cast<boost::chrono::milliseconds> (
		boost::chrono::high_resolution_clock::now() - start) << std::endl;

	system("pause");
	return 0;
}
