#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

int main()
{
	
	std::string s = "Content Length: 123";

	boost::regex expr("\\d+$");
	boost::smatch match;

	int num = 0;

	if (boost::regex_search(s, match, expr)) 
	{
		num = boost::lexical_cast<int>(match[0]);
	}

	return 0;
}