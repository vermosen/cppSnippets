#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <string>
#include <iostream>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

int main(void) {
	std::string input("1234, 6543, 42");
	std::string::const_iterator begin = input.begin(), end = input.end();

	qi::rule<
		std::string::const_iterator,
		int(),							//output (_val) and input (_r1)
		qi::locals<int>,             //local int (_a)
		ascii::space_type
	>
		intRule =
		qi::int_[qi::_a = qi::_1]					//local = x1
		>> ","
		>> qi::int_[qi::_a += qi::_1]				//local = x1 + x2
		>> ","
		>> qi::int_
		[
			qi::_val = qi::_a*qi::_1 //output = local*x3 + x0
		];

	int ruleValue, x0 = 10;
	qi::phrase_parse(begin, end, intRule, ascii::space, ruleValue);
	std::cout << "rule value: " << ruleValue << std::endl;
	return 0;
}