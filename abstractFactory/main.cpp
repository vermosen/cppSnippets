#include <iostream>
#include <string>
#include <tuple>

#include <boost/shared_ptr.hpp>

# include "registrable/child.hpp"

// an example of abstract factory with auto-registration of the classes
int main() {

	try
	{
		boost::shared_ptr<base> myBase = factory<base, std::tuple<int, std::string> >::createInstance(std::make_tuple(1, "A"));
		myBase->foo();
	}
	catch (const std::exception&)
	{
		std::cout << "Oups, something went wrong..." << std::endl;
	}

	system("pause");

}