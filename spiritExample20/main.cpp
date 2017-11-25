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
		int age;
		std::string surname;
		std::string forename;
		boost::optional<double> salary;
	};
	//]
}

//[tutorial_employee_adapt_struct
BOOST_FUSION_ADAPT_STRUCT(
	test::record,
	(int, age)
	(std::string, surname)
	(std::string, forename)
	(boost::optional<double>, salary)
)
//]

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
			using qi::_val;
			using qi::_1;
			using qi::lit;
			using qi::double_;
			using qi::lexeme;
			using ascii::char_;

			rString
				%= lexeme[+(char_ - lit("\",\""))];

			rOptString
				%= lit("")
					|
				rString;

			rOptDouble
				%= -(double_);

			rRecord 
				%= lit("record")
				>> "{\""
				>> int_		>> "\",\""
				>> rString	>> "\",\""
				>> rString >> "\",\""
				>> rOptDouble
				>> "\"}"
				;

			rRecords = +(rRecord >> qi::eol);

			rOptDouble.name("rOptDouble");
			BOOST_SPIRIT_DEBUG_NODES((rOptDouble))
			debug(rOptDouble);
		}

		qi::rule<Iterator, std::string(), ascii::blank_type> rString;
		qi::rule<Iterator, std::string(), ascii::blank_type> rOptString;
		qi::rule<Iterator, boost::optional<double>(), ascii::blank_type> rOptDouble;
		qi::rule<Iterator, record(), ascii::blank_type> rRecord;
		qi::rule<Iterator, std::vector<record>(), ascii::blank_type> rRecords;
	};
	//]
}

static const std::string recs("record{\"36\",\"vermosen\",\"jean-mathieu\",\"\"}\n");
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
	typedef std::string::const_iterator iterator_type;
	typedef test::record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	start = boost::chrono::high_resolution_clock::now();

	std::vector<test::record> emps;
	std::string::const_iterator iter = recs.begin();
	std::string::const_iterator end = recs.end();

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
