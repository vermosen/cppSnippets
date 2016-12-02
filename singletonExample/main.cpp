#include <iostream>

#include "templateClass.hpp"

class myImpl
{
public:
	void mySubMethod() { std::cout << "hello world !" << std::endl; }
};

int main() 
{
	myExample<myImpl> test;
	test.myMethod();
	return 0;
}