#include<iostream>
#include<chrono>

constexpr long int cFactorial(long int x)
{
    long int suma=1;
    for(long int i = 1; i <= x; i++)
    {
        suma *= i;
    }
    
    return suma;
}

long int factorial(long int x)
{
    long int suma=1;
    for(long int i = 1; i <= x; i++)
    {
        suma *= i;
    }
    
    return suma;
}

using ms = std::chrono::nanoseconds;
using get_time = std::chrono::steady_clock;

int main(){
    long int x = 100;

    std::cout<<"Now calling the constexpr"<<std::endl;
    auto start1 = get_time::now();
    
    for (int i = 0; i < x; i++)
    {
        cFactorial(200);
    }
    
    auto end1 = get_time::now();
    
    std::cout<<"Now calling the normal"<<std::endl;
    auto start2 = get_time::now();
    
    for (int i = 0; i < x; i++)
    {
        factorial(200);
    }
    
    auto end2 = get_time::now();
    std::cout<<"Elapsed time for constexpr is "<< std::chrono::duration_cast<ms>(end1-start1).count()
    <<" Elapsed time for normal is "<< std::chrono::duration_cast<ms>(end2-start2).count()<<std::endl;
    
    return EXIT_SUCCESS;
}