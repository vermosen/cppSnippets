#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <boost/shared_ptr.hpp>

# include "registrable/child.hpp"

// void argument list
int main() 
{    
    try
    {
        // parametric ctor
        auto c = factory<base, int, std::string>::createInstance(1, "A");

        c->foo();
        
        // parameterless ctor
        c = factory<base, int>::createInstance(1);

        c->foo();        
    }
    catch (const std::exception & ex)
    {
        std::cout << "something went wrong: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}