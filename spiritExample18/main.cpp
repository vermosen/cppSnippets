#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/optional.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <map>

#define BOOST_SPIRIT_DEBUG

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct record
{
	boost::optional<bool> value;
};

BOOST_FUSION_ADAPT_STRUCT(
	record,
	(boost::optional<bool>, value)
)

using namespace boost::spirit;

struct custom_bool_policy
{
	template <typename iterator, typename Attribute>
	static bool
		parse_true(iterator& first, iterator const& last, Attribute& attr)
	{
		if (qi::detail::string_parse("Y", first, last, qi::unused))
		{
			traits::assign_to(true, attr);    // result is true
			return true;
		}
		return qi::bool_policies<bool>::parse_true(first, last, attr);
	}

	template <typename iterator, typename Attribute>
	static bool
		parse_false(iterator& first, iterator const& last, Attribute& attr)
	{
		if (qi::detail::string_parse("N", first, last, qi::unused))
		{
			traits::assign_to(false, attr);   // result is false
			return true;
		}
		return qi::bool_policies<bool>::parse_false(first, last, attr);
	}
};

template <typename iterator, typename skipper>
struct recordGrammar : qi::grammar<iterator, record(), skipper>
{
	recordGrammar() : recordGrammar::base_type(rBool)
	{
		rBool 
			%= "\"" >> *(qi::lit(' ')) >> "\""
				| 
			qi::lexeme["\"" >> pBool >>  "\""];
	}

	//qi::rule<iterator, std::string(), skipper> atom;
	qi::rule<iterator, record(), skipper> rBool;

	qi::bool_parser<bool, custom_bool_policy> pBool;
};

static std::vector<std::string> recs = { "\"Y\"", "\" \"", "\"\"", "\"N\""};

int main()
{
	typedef std::string::iterator iterator_type;

	recordGrammar<iterator_type, ascii::space_type> g; // Our grammar

	for(auto & rec : recs)
	{
		record res;
		iterator_type iter = rec.begin(), end = rec.end();

		if (phrase_parse(iter, end, g, ascii::space, res) && iter == end)
		{
			if (res.value == boost::none)
			{
				std::cout << "the value is null" << std::endl;
			}
			else
			{
				if (res.value.get() == true)
				{
					std::cout << "the value is true" << std::endl;
				}
				else
				{
					std::cout << "the value is false" << std::endl;
				}
			}
		}
		else
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "-------------------------\n";
		}
	}

	system("pause");
	return 0;
}
