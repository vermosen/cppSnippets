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
        // testing the create functions
        base * a = create<base, child, std::string>("A");                       // create a pointer on C by calling the ctor of child with parameter "A"
        a->foo();
        
        base * b = create<base, child>();                                       // same but with the default ctor
        b->foo();
        
        // testing the dummy registration structs
        dummy<base, int, child, std::string> d1;
        d1(1, "hello world!");
        
        dummy<base, int, child> d2;
        d2(1);        
    }
    catch (const std::exception & ex)
    {
        std::cout << "something went wrong: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}