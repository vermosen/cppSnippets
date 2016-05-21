/*

	implement the example pp 55-56 of
	Beyond the C++ standard library
	from Bjorn Karlsson

*/

#include "class.hpp"

#include <boost/cast.hpp>

// a quick test to test boost cast functionalities
int main(int argc, char** argv) {

	base1 * a = new child();
	
	// calling the two doStuff1 functions...
	a->base1::doStuff1();
	a->doStuff1();

	try {

		// first, a downcast
		child * b = boost::polymorphic_cast<child *>(a);

		// calling all the possible functions
		b->base1::doStuff1();
		b->doStuff1();
		b->base2::doStuff2();
		b->doStuff2();

		// then a cross-cast
		base2 * c = boost::polymorphic_cast<base2 *>(a);

		c->base2::doStuff2();
		c->doStuff2();

	}
	catch (std::bad_cast & e) {
	
		std::cout << e.what() << std::endl;
	
	}

	// wait for a return
	std::cout << "Press any key..." << std::endl;
	std::cin.get();
	return 0;

}
