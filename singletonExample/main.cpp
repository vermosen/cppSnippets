#include <iostream>

#include "templateClass.hpp"

class myImpl
{
public:
	static void mySubMethod() { std::cout << "hello world too !" << std::endl; }
};

int main() 
{
	myExample<myImpl> test;
	test.myMethod();
	test.myFriendMethod();

	__debugbreak();
	return 0;
}