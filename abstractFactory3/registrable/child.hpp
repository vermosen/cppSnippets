#ifndef child_hpp
#define child_hpp

#include <iostream>

#include "base.hpp"

class child : public base 
{
public:
	child(const std::tuple<int, int> & t) : base(), i_(std::get<0>(t)), j_(std::get<1>(t)) {}
	virtual void foo() { std::cout << "It's alive: " << i_ + j_ << std::endl; }
private:
	int i_;
	int j_;
	static registerType < base, std::string, child, std::tuple<int, int> > register_;
};

#endif
