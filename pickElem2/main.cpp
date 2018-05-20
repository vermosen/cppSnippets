#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_USE_PHOENIX_V3

#include <boost/chrono.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

namespace test
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phx = boost::phoenix;

	struct record
	{
		std::string surname;
		std::string forename;
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	test::record,
	(std::string, surname)
	(std::string, forename)
)

namespace test {

	template <typename It, typename Skipper>
	struct record_parser : qi::grammar<It, void(std::vector<record>, std::string), Skipper> {
		record_parser() : record_parser::base_type(rRecords)
		{
			rString
				= qi::lexeme[*(qi::char_ - qi::char_(",}"))];

			rRecord
				= qi::lit("record")
				>> "{"
				>> rString >> ","
				>> rString >> "}"
				;

			rRecords
				= rRecord(qi::_r2)[phx::push_back(qi::_r1, qi::_1)]
				>> *("," >> rRecord(qi::_r2)[phx::push_back(qi::_r1, qi::_1)]);
		}

		qi::rule<It, std::string(), Skipper> rString;
		qi::rule<It, record(std::string), Skipper> rRecord;
		qi::rule<It,void(std::vector<record>, std::string), Skipper> rRecords;
	};
}

static std::string recs("record{bob,dole},record{john,doo},record{john,smith}");

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////

int main()
{
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "\t\tAn employee parser for Spirit...\n\n";
	std::cout << "/////////////////////////////////////////////////////////\n\n";

	typedef std::string::iterator iterator_type;
	typedef test::record_parser<iterator_type, qi::space_type> record_parser;

	record_parser g; // Our grammar

	std::string s = "john";
	std::vector<test::record> emps;
	iterator_type iter = recs.begin();
	iterator_type end = recs.end();

	bool r = boost::spirit::qi::phrase_parse(iter, end, g(phx::ref(emps), phx::ref(s)), qi::space);

	if (r) {
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "got: " << emps.size() << " employees" << std::endl;
		std::cout << "\n-------------------------\n";
	}
	else {
		std::cout << "-------------------------\n";
		std::cout << "Parsing failed\n";
		std::cout << "-------------------------\n";
	}

	return 0;
}
