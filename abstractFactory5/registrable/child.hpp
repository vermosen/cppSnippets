#ifndef child_hpp
#define child_hpp

#include <iostream>

#include "registrable/base.hpp"

class child : public base 
{
public:
    child(std::string str, int i) : base(), i_(i) {}
    virtual void foo() { std::cout << "It's alive: " << i_ << std::endl; }
private:
    int i_;
    static registerType <base, std::string, child, std::string, int> register_;
};

//class parameterlessChild : public base
//{
//public:
//    parameterlessChild() : base() {}
//    virtual void foo() {std::cout << "it's alive too !" << std::endl; }
//private:
//    static registerType <base, std::string, child, void> register_;  
//};

#endif
