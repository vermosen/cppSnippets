#ifndef child_hpp
#define child_hpp

#include <iostream>

#include "registrable/base.hpp"

class child : public base 
{
public:
    child(std::string str) : base(), str_(str) {}
    child() : base(), str_("HA ! HA ! HA !") {}
    virtual void foo() { std::cout << "It's alive: " << str_ << std::endl; }
private:
    std::string str_;
    static registerType <base, int, child, std::string> register1_;
    //static registerType <base, int, child> register2_;
};

#endif
