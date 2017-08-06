#ifndef child_hpp
#define child_hpp

#include <iostream>

#include "registrable/base.hpp"

class child : public base 
{
public:
	child(int i) : base(), i_(i) {}
	virtual void foo() { std::cout << "It's alive: " << i_ << std::endl; }
private:
	int i_;
	static registerType < base, std::string, child, int> register_;
};

#endif
