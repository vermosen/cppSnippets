#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <boost/shared_ptr.hpp>

# include "registrable/child.hpp"

// an example of abstract factory with auto-registration of the classes
int main() {

	try
	{
		std::vector<boost::shared_ptr<base> > vec;
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "A")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "B")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "C")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "A")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "C")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "E")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "D")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "B")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "B")));
		vec.push_back(factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "C")));

		for (auto it = vec.cbegin(); it != vec.cend(); it++)
		{
			it->get()->foo();
		}
		
	}
	catch (const std::exception&)
	{
		std::cout << "Oups, something went wrong..." << std::endl;
		return 1;
	}

	return 0;

}
