#ifndef child_hpp
#define child_hpp

#include <iostream>

#include "registrable/base.hpp"

class child : public base 
{
public:
	virtual void foo() { std::cout << "It's alive !" << std::endl; }
private:
	static registerType < base, std::tuple<int, std::string>, child> register_;
};

#endif
