#include <iostream>

#include <adept.h>

int main()
{
    using adept::adouble;
    adept::Stack s;

    adouble x[2]; // Our independent variables
    adouble y;    // Our dependent variable

    // Set the values of x
    x[0] = 2.0;
    x[1] = 3.0;

    double x_r[2];
    x_r[0] = x[0].value();
    x_r[1] = x[1].value();

    return 0;
}