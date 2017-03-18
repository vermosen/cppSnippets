#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <boost/shared_ptr.hpp>

# include "registrable/child.hpp"

// an example of abstract factory with auto-registration of the classes
int main() 
{
	try
	{
		boost::shared_ptr<base> c = factory<base, std::string, std::string, int>::createInstance("A", "A", 1);

		c->foo();
	}
	catch (const std::exception&)
	{
		std::cout << "Oups, something went wrong..." << std::endl;
		return 1;
	}

	return 0;

}
