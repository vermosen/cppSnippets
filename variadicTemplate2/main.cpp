/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vermosen
 *
 * Created on August 7, 2017, 2:51 PM
 */

#include <cstdlib>
#include <map>
#include <iostream>

// create a pointer of C by calling the constructor of T with Args
template <typename C, typename T, typename ... Args> 
C * create(Args ... args)
{
    return new T(args ...);
}

struct base
{
    base(int i) : i_(i) {}
    int i() const { return i_; }
private:
    int i_;
};

struct child1 : public base
{
public:
    child1(int i) : base(i) {}
};

struct child2 : public base
{
public:    
    child2() : base(-1) {}
};

// minimal example to show how empty variadic parameters actually unfold correctly
int main(int argc, char** argv)
{
    try
    {
        base * t1 = create<base, child1, int>(2);                               // would not compile with child2
        base * t2 = create<base, child2>();                                     // would not compile with child1
        
        std::cout 
            << t1->i() 
            << ", " 
            << t2->i() 
            << std::endl;
    }
    catch(std::exception & ex)
    {
        std::cout << " an error has occurred: " << ex.what() << std::endl;
    }
    
    return 0;
}

