#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <boost/shared_ptr.hpp>

# include "registrable/child.hpp"

// trace info
void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

// void argument list
int main() 
{
    // install the handler
    signal(SIGSEGV, handler);
    
    try
    {
        boost::shared_ptr<base> c = factory<base, std::string, std::string, int>::createInstance("A", "A", 1);

        c->foo();
    }
    catch (const std::exception & ex)
    {
        std::cout << "something went wrong: " << ex.what() << std::endl;
        return 1;
    }

    return 0;

}