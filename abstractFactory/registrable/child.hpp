#ifndef child_hpp
#define child_hpp

#include <iostream>

#include "base.hpp"

class child1 : public base 
{
public:
    virtual void foo() { std::cout << "It's alive !" << std::endl; }
private:
    static registerType < base, std::tuple<int, std::string>, child1> register_;
};

class child2 : public base
{
public:
    virtual void foo() { std::cout << "It's alive !" << std::endl; }
private:
    static registerType < base, std::tuple<int, std::string>, child2> register_;
};

class child3 : public base
{
public:
    virtual void foo() { std::cout << "It's alive !" << std::endl; }
private:
    static registerType < base, std::tuple<int, std::string>, child3> register_;
};

class child4 : public base
{
public:
    virtual void foo() { std::cout << "It's alive !" << std::endl; }
private:
    static registerType < base, std::tuple<int, std::string>, child4> register_;
};

class child5 : public base
{
public:
    virtual void foo() { std::cout << "It's alive !" << std::endl; }
private:
    static registerType < base, std::tuple<int, std::string>, child5> register_;
};

#endif
