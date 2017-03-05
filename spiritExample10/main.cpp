#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <map>

namespace qi = boost::spirit::qi;

typedef std::map<std::string, std::string> target;

template <typename iterator>
struct record_parser : qi::grammar<iterator, target()>
{
	record_parser() : record_parser::base_type(query)
	{
		query = pair >> *(qi::lit(';') >> pair);
		pair = key >> -('=' >> value);
		key = qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
		value = +qi::char_("a-zA-Z_0-9");
	}

	qi::rule<iterator, target()> query;
	qi::rule<iterator, std::pair<std::string, std::string>()> pair;
	qi::rule<iterator, std::string()> key, value;
};

static std::string rec("key1=pair1;key2=pair2");

int main()
{
	typedef std::string::iterator iterator_type;
	typedef record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	target emp;
	iterator_type iter = rec.begin();
	iterator_type end = rec.end();

	if (qi::parse(iter, end, g, emp) && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		for (auto it = emp.begin(); it != emp.end(); it++)
			std::cout << "got: " << it->first << ", " << it->second << std::endl;

		std::cout << "\n-------------------------\n";
	}
	else
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing failed\n";
		std::cout << "-------------------------\n";
	}

	system("pause");
	return 0;
}
