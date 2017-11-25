/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vermosen
 *
 * Created on September 16, 2017, 3:41 PM
 */

#include <iostream>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/static_assert.hpp>

typedef boost::mpl::vector<char, short, int, long, float, double> types;

typedef long t_find;

typedef boost::mpl::find<types, t_find>::type pos;

// x is of long type
BOOST_STATIC_ASSERT((boost::is_same<boost::mpl::deref<pos>::type, long>::value));

typedef float t_next;

typedef boost::mpl::next<pos>::type p_next;

// x precede a float type
BOOST_STATIC_ASSERT((boost::is_same<boost::mpl::deref<p_next>::type, t_next>::value));

int main(int argc, char** argv)
{
    
}

