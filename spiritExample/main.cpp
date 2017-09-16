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

namespace test
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	///////////////////////////////////////////////////////////////////////////
	//  Our employee struct
	///////////////////////////////////////////////////////////////////////////
	//[tutorial_employee_struct
	struct record
	{
		int age;
		std::string surname;
		std::string forename;
		double salary;
	};
	//]
}

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen. This has to
// be in global scope.

//[tutorial_employee_adapt_struct
BOOST_FUSION_ADAPT_STRUCT(
	test::record,
	(int, age)
	(std::string, surname)
	(std::string, forename)
	(double, salary)
)
//]

namespace test
{
	///////////////////////////////////////////////////////////////////////////////
	//  Our employee parser
	///////////////////////////////////////////////////////////////////////////////
	//[tutorial_employee_parser
	template <typename Iterator>
	struct record_parser : qi::grammar<Iterator, record(), ascii::space_type>
	{
		record_parser() : record_parser::base_type(start)
		{
			using qi::int_;
			using qi::lit;
			using qi::double_;
			using qi::lexeme;
			using ascii::char_;

			quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];

			start %=
				lit("record")
				>> '{'
				>> int_ >> ','
				>> quoted_string >> ','
				>> quoted_string >> ','
				>> double_
				>> '}'
				;
		}

		qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
		qi::rule<Iterator, record(), ascii::space_type> start;
	};
	//]
}

static const std::string rec("record{36, \"vermosen\", \"jean-mathieu\", 160000}");
boost::chrono::high_resolution_clock::time_point start;

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////

int main()
{
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "\t\tAn employee parser for Spirit...\n\n";
	std::cout << "/////////////////////////////////////////////////////////\n\n";

	using boost::spirit::ascii::space;
	typedef std::string::const_iterator iterator_type;
	typedef test::record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	start = boost::chrono::high_resolution_clock::now();
	int i = 0;  while (i++ < 1000000)
	{
		test::record emp;
		std::string::const_iterator iter = rec.begin();
		std::string::const_iterator end = rec.end();

		if (phrase_parse(iter, end, g, space, emp) && iter == end)
		{
			std::cout << boost::fusion::tuple_open('[');
			std::cout << boost::fusion::tuple_close(']');
			std::cout << boost::fusion::tuple_delimiter(", ");

			std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded\n";
			std::cout << "got: " << boost::fusion::as_vector(emp) << std::endl;
			std::cout << "\n-------------------------\n";
		}
		else
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "-------------------------\n";
		}
	}

	std::cout << "Conversion done in " << boost::chrono::duration_cast<boost::chrono::milliseconds> (
		boost::chrono::high_resolution_clock::now() - start) << std::endl;

	system("pause");
	return 0;
}
