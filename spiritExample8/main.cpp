#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>

using namespace boost::spirit;

namespace fusion = boost::fusion;

typedef boost::gregorian::date date;
typedef fusion::vector<int, int, int> adapter;

struct errorHandler
{
	errorHandler() = default;
	errorHandler(errorHandler const&) = delete;

	template<typename...> struct result { typedef void type; };
	template<typename Iter> void operator()(
		Iter & first_iter, Iter last_iter,
		Iter error_iter, const qi::info& what) const
	{
		// skip the faulty record...
		if (first_iter != last_iter)
		{
			while (*(first_iter++) != '\n');

			int i = 0;
		}
	}
};

template<>
struct traits::transform_attribute<date, adapter, qi::domain>
{
	typedef adapter type;
	static type pre(date a) { return type(); }
	static void post(date & d, type const& v)
	{
		d = date(fusion::at_c<0>(v), fusion::at_c<1>(v), fusion::at_c<2>(v));
	}

	static void fail(date& dt) {}
};

template <typename Iterator>
struct parser : qi::grammar<Iterator, std::vector<date>(), ascii::blank_type>
{
	parser() : parser::base_type(rDates)
	{
		rDate %= '(' >
			qi::int_[_pass = (_1 >= 1400 && _1 < 10000)] > '-' >
			qi::int_[_pass = (_1 > 0 && _1 <= 12)] > '-' >
			qi::int_[_pass = (_1 > 0 && _1 <= 31)] > ')' >> qi::eol;

		rDates %= +(rDate/* >> qi::eol*/) >> qi::eoi;

		qi::on_error<qi::retry>
		(	
			rDate, boost::phoenix::bind(
				boost::phoenix::ref(errorHandler_),
					qi::_1, qi::_2, qi::_3, qi::_4)
		);
	}

	qi::rule<Iterator, adapter(), ascii::blank_type> rDate;
	qi::rule<Iterator, std::vector<date>(), ascii::blank_type> rDates;

	errorHandler errorHandler_;
};

static std::string emps("(2016-01-02)\n(2016-01-05)\n(2016-42-05)\n");

int main()
{
	using boost::spirit::ascii::blank;

	parser<std::string::iterator> g;

	std::vector<date> recs;
	std::string::iterator iter = emps.begin(), end = emps.end();

	if (phrase_parse(iter, end, g, blank, recs) && iter == end)
	{
		for (auto it = recs.begin(); it != recs.end(); it++)
			std::cout << "got: " << *it << std::endl;
	}
	else
	{
		std::cout << "Parsing failed\n";
	}

	return 0;
}
