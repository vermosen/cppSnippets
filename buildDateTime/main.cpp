
#define BOOST_SPIRIT_DEBUG 

#include <ostream>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/std_pair.hpp> 
#include <boost/phoenix/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

struct date {

	date() {}
	date(unsigned int y, unsigned short m, unsigned short d,
		long h, long mm, long s, unsigned int f) : m_y(y) {}

	friend std::ostream& operator<<(std::ostream& os, const date& dt);

protected:
	unsigned int m_y;
};

std::ostream& operator<<(std::ostream& os, const date& dt)
{
	os << dt.m_y;
	return os;
}

struct aggregate {

	date m_dt;
	std::string m_s1;
	std::string m_s2;
};

BOOST_FUSION_ADAPT_STRUCT(
	aggregate,
	(date, m_dt)
	(std::string, m_s1)
	(std::string, m_s2)
)

namespace parsers {

	// parse the message into a termstructure object
	template <typename Iterator>
	struct parse_date : qi::grammar<Iterator, aggregate()> {

		parse_date() : parse_date::base_type(rStart) {

			rDate =
				(
					   qi::uint_ >> '/'
					>> qi::ushort_ >> '/'
					>> qi::ushort_ >> ' '
					>> qi::int_ >> ':'
					>> qi::int_ >> ':'
					>> qi::int_ >> '.'
					>> qi::uint_
				)
				[
					qi::_val = phx::construct<date>(
						qi::_1, qi::_2, qi::_3, 
						qi::_4, qi::_5, qi::_6, qi::_7)
				];	

			rStart 
				%= rDate
				>> qi::omit[*(!qi::lit("INFO") >> qi::char_)
					>> qi::lit("INFO")] >> ' '
				>> qi::lexeme[*(qi::char_ - ':')] >> ':'
				>> qi::lexeme[*(qi::char_ - ':')] >> ':'
				//>> rMap;
				;

			rStart.name("rStart");
			BOOST_SPIRIT_DEBUG_NODES((rStart));
			debug(rStart);

			rDate.name("rDate");
			BOOST_SPIRIT_DEBUG_NODES((rDate));
			debug(rDate);
		}

		qi::rule<Iterator, aggregate()> rStart;
		qi::rule<Iterator, date()> rDate;
	};
}

int main() {

	parsers::parse_date<std::string::iterator> grammar;

	std::string s("2018/04/20 08:20:10.123456 (garbage) INFO APPL:APPL:");
	aggregate d;

	bool result = qi::phrase_parse(
		s.begin(), s.end()
		, grammar, qi::space
		, d);

	return 0;
}