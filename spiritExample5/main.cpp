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

int main()
{
	std::string num = "12,313.12346";
	std::string::const_iterator first = num.cbegin();
	std::string::const_iterator last = num.cend();

	qi::uint_parser<unsigned, 10, 1, 3> uint3;
	qi::uint_parser<unsigned, 10, 3, 3> uint3_3;
	qi::uint_parser<unsigned, 10, 1, 1> uint1_1; // max 6 digits

	double result = 0;

	if (parse(first, last, uint3, result))
	{
		double n = 0;
		while (*first != '.' && qi::parse(first, last, ',') && qi::parse(first, last, uint3_3, n))
		{
			result = result * 1000 + n;
		}

		qi::parse(first, last, '.');

		int i = 10;
		while (qi::parse(first, last, uint1_1, n))
		{
			result = result + n / i;
			i *= 10;
		}

		std::cout << std::fixed << std::setprecision(6) << result << std::endl;

		system("pause");
		return 0;
	}

	return 0;
}
