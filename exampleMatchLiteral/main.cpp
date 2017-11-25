#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

int main()
{

	std::string s = "Content Length: 123\r";

	boost::regex expr("(Content Length: )(\\d+)(\r)");
	boost::smatch match;

	int num = 0;

	if (boost::regex_search(s, match, expr))
	{
		std::string t = match[2];
		num = boost::lexical_cast<int>(t);
	}

	return 0;
}