#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

#include <iostream>
#include <string>
#include <map>

namespace qi = boost::spirit::qi;

typedef std::pair<std::string, std::string> pair;
typedef std::map<std::string, std::string> map;

template <typename iterator>
struct record_parser : qi::grammar<iterator, pair()>
{
	record_parser() : record_parser::base_type(pair)
	{
		pair = key >> -('=' >> value);
		key = "-" >> qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
		value = +qi::char_("a-zA-Z_0-9");
	}

	qi::rule<iterator, pair()> pair;
	qi::rule<iterator, std::string()> key, value;
};

static std::vector<std::string> rec{"-key1=pair1", "-key2=pair2"};

int main()
{
	typedef std::string::iterator iterator_type;
	typedef record_parser<iterator_type> record_parser;

	record_parser g; // Our grammar

	map emp;

	for (auto & it = rec.begin(); it != rec.end(); it++)
	{
		pair keyval;
		iterator_type iter = it->begin();
		iterator_type end = it->end();

		if (qi::parse(iter, end, g, keyval) && iter == end)
		{
			emp.insert(keyval);
		}
		else
		{
			std::cout << "failed to insert" << *it;
		}
	}

	std::cout << "Parsing succeeded\n";
	for (auto it = emp.begin(); it != emp.end(); it++)
		std::cout << "got: " << it->first << ", " << it->second << std::endl;

	system("pause");
	return 0;
}
