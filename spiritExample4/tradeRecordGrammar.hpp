#ifndef TRADE_RECORD_GRAMMAR_HPP_
#define TRADE_RECORD_GRAMMAR_HPP_

#include <string>
#include <tuple>

// 3 hours wasted on this sh.t !!!!!
#define FUSION_MAX_VECTOR_SIZE 20

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/none.hpp>
#include <boost/optional/optional_io.hpp>

#include "tradeRecord.hpp"
#include "pattern/enumManager.hpp"

using namespace boost::spirit;

BOOST_FUSION_ADAPT_STRUCT(
	dtcc::database::record,
	(int, DISSEMINATION_ID)
	(boost::optional<int>, ORIGINAL_DISSEMINATION_ID)
	(dtcc::database::action, ACTION)
	(dtcc::database::tPTime, EXECUTION_TIMESTAMP)
	(dtcc::database::cleared, CLEARED)
	(dtcc::database::collateralization, INDICATION_OF_COLLATERALIZATION)
	(boost::optional<bool>, INDICATION_OF_END_USER_EXCEPTION)
	(bool, INDICATION_OF_OTHER_PRICE_AFFECTING_TERM)
	(bool, BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS)
	(dtcc::database::tDate, EFFECTIVE_DATE)
	(dtcc::database::tDate, END_DATE)
)

template<>
struct boost::spirit::traits::transform_attribute<dtcc::database::collateralization, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::collateralization a) { return type(); }

	static void post(dtcc::database::collateralization & d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::database::collateralization>::toEnum(v);
	}

	static void fail(dtcc::database::collateralization &) {}
};

template<>
struct boost::spirit::traits::transform_attribute<dtcc::database::cleared, char, boost::spirit::qi::domain>
{
	typedef char type;

	static type pre(dtcc::database::cleared a) { return type(); }

	static void post(dtcc::database::cleared& d, type const& v)
	{
		d = static_cast<dtcc::database::cleared>(v == 'C');
	}

	static void fail(dtcc::database::cleared&) {}
};

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::action, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::action a) { return type(); }

	static void post(dtcc::database::action& d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::database::action>::toEnum(v);
	}

	static void fail(dtcc::database::action&) {}
};

template<>
struct traits::transform_attribute<dtcc::database::tDate, dtcc::database::dateAdaptator, qi::domain>
{
	typedef dtcc::database::dateAdaptator type;

	static type pre(dtcc::database::tDate a) { return type(); }

	static void post(dtcc::database::tDate& d, type const& v)
	{
		(v ? d = boost::gregorian::date(boost::get<0>(*v), boost::get<1>(*v), boost::get<2>(*v)) : d = boost::none);
	}

	static void fail(dtcc::database::tDate&) {}
};

template<>
struct traits::transform_attribute<dtcc::database::tPTime, dtcc::database::pTimeAdaptator, qi::domain>
{
	typedef dtcc::database::pTimeAdaptator type;

	static type pre(dtcc::database::tPTime a) { return type(); }

	static void post(dtcc::database::tPTime& d, type const& v)
	{
		d = boost::posix_time::ptime(
			boost::gregorian::date(boost::get<0>(v), boost::get<1>(v), boost::get<2>(v)),
			boost::posix_time::time_duration(boost::get<3>(v), boost::get<4>(v), boost::get<5>(v)));
	}

	static void fail(dtcc::database::tPTime&) {}
};

template <typename Iterator>
struct record_parser : qi::grammar<Iterator, dtcc::database::record(), ascii::space_type>
{
	record_parser() : record_parser::base_type(start)
	{
		rInt		
			%= qi::lexeme['"' >> int_ >> '"'];

		rOptInt
			%= ascii::no_case["\"\""]
					|
				qi::lexeme['"' >> (qi::int_) >> '"'];

		rAction 
			%= qi::lexeme['"' >> +(ascii::char_ - '"') >> '"'];

		rTime 
			%= qi::lexeme['"' >> qi::int_ >> "-" >> qi::int_ >> "-" >> qi::int_ >> 'T' >>
			qi::int_ >> ":" >> qi::int_ >> ":" >> qi::int_ >> '"'];

		rDate 
			%= ascii::no_case["\"\""] 
					| 
				qi::lexeme['"' >> qi::int_ >> "-" >> qi::int_ >> "-" >> qi::int_ >> '"'];
		
		rCleared 
			%= qi::lexeme['"' >> (ascii::char_("CU")) >> '"'];

		rIndOfCollat 
			%= qi::lexeme['"' >> (ascii::char_("FOPU")) >> (ascii::char_("C")) >> '"'];
		
		rOptBool 
			%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("Y")[_val = true			] |
														qi::lit("N")[_val = false			] |
														qi::lit("" )[_val = boost::none])	] >> '"'];
		rBool 
			%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("Y")[_val = true] |
														qi::lit("N")[_val = false])] >> '"'];

		start %=
			rInt >> ',' >>
			rOptInt >> ',' >>
			rAction >> ',' >>
			rTime >> ',' >>
			rCleared >> ',' >>
			rIndOfCollat >> ',' >>
			rOptBool >> "," >>
			rBool >> "," >>
			rBool >> "," >>
			rDate >> ',' >>
			rDate
			;
	}

	qi::rule<Iterator, int(), ascii::space_type> rInt;
	qi::rule<Iterator, int(), ascii::space_type> rOptInt;
	qi::rule<Iterator, dtcc::database::pTimeAdaptator(), ascii::space_type> rTime;
	qi::rule<Iterator, dtcc::database::dateAdaptator(), ascii::space_type> rDate;
	qi::rule<Iterator, std::string(), ascii::space_type> rAction;
	qi::rule<Iterator, char(), ascii::space_type> rCleared;
	qi::rule<Iterator, std::string(), ascii::space_type> rIndOfCollat;
	qi::rule<Iterator, boost::optional<bool>(), ascii::space_type>	rOptBool;
	qi::rule<Iterator, bool(), ascii::space_type> rBool;
	qi::rule<Iterator, dtcc::database::record(), ascii::space_type> start;
};

#endif
