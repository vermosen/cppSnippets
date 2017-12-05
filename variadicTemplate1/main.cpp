#include <iostream>
#include <typeinfo>


// recursive unpacking
template <typename ... Args>             
struct impl;                         // fwd declaration

template <typename First, typename ... Args>
struct impl<First, Args...>
{
    static std::string name()
    {
        return std::string(typeid(First).name()) + " " + impl<Args...>::name();
    }
};

template <>
struct impl <>
{
    static std::string name()
    {
        return "";
    }
};

template <typename ... Args>
std::string type_name(Args ... args)
{
    return impl<Args ...>::name();
};

int main()
{
    try
    {
        int i = 1, j = 2, k = 3;

        std::cout << "types: " << type_name(i, j, k) << std::endl;
    }
    catch(std::exception & ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}