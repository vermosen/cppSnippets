#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi_skip.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/chrono.hpp>
#include <boost/phoenix/bind.hpp>

#include <iostream>
#include <string>
#include <complex>

#define BOOST_SPIRIT_DEBUG

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
	// our recovery handler
	template <typename = void> struct errorHandler
	{
		errorHandler() = default;
		errorHandler(errorHandler const&) = delete;

		template<typename...> struct result { typedef void type; };
		template<typename Iter> void operator()(
			Iter & first_iter, Iter last_iter,
			Iter error_iter, const qi::info& what) const
		{
			// retrieve the beginning of the record
			auto temp = first_iter;

			if (first_iter != last_iter)
			{
				while (*(first_iter++) != '\n');

				// log the error
				std::cout << "wrong record: " << std::string(temp, first_iter) << std::endl;
			}
		}
	};

	template <typename Iterator>
	struct record_parser : qi::grammar<Iterator, std::vector<record>(), ascii::blank_type>
	{
		record_parser() : record_parser::base_type(rRecords)
		{
			using qi::int_;
			using qi::lit;
			using qi::double_;
			using qi::lexeme;
			using ascii::char_;

			rQuotedString %= lexeme['"' >> +(char_ - '"') >> '"'];

			rRecord %= qi::eps
				>> lit("record")
				> '{'
				> int_ > ','
				> rQuotedString > ','
				> rQuotedString > ','
				> double_
				> '}' 
				> qi::eol
				;

			rRecords = +(rRecord) >> qi::eoi;

			BOOST_SPIRIT_DEBUG_NODES((rRecord))
			BOOST_SPIRIT_DEBUG_NODES((rRecords))

			// TODO: pass reference for recovery
			qi::on_error<qi::retry>
			(
				// single record error we log the error
				rRecord, boost::phoenix::bind(boost::phoenix::ref(errorHandler_),
					boost::spirit::qi::_1,
					boost::spirit::qi::_2, // it start
					boost::spirit::qi::_3, // it end
					boost::spirit::qi::_4) // what failed
			);
		}

		qi::rule<Iterator, std::string(), ascii::blank_type> rQuotedString;
		qi::rule<Iterator, record(), ascii::blank_type> rRecord;
		qi::rule<Iterator, std::vector<record>(), ascii::blank_type> rRecords;

		errorHandler<> errorHandler_;
	};
	//]
}

static const std::string emps("record{36 \"jim\", \"smith\", 15000}\nrecord{36, \"bob\", \"smith\", 10000}\nrecord{36 \"rob\", \"smith\", 10000}\n");
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

	std::vector<test::record> recs;
	std::string::const_iterator iter = emps.begin();
	std::string::const_iterator end = emps.end();

	if (phrase_parse(iter, end, g, blank, recs) && iter == end)
	{
		std::cout << boost::fusion::tuple_open('[');
		std::cout << boost::fusion::tuple_close(']');
		std::cout << boost::fusion::tuple_delimiter(", ");

		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";

		for (auto it = recs.begin(); it != recs.end(); it++)
			std::cout << "got: " << boost::fusion::as_vector(*it) << std::endl;

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
