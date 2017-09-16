#include <daysOfWeek.hpp>

int main() 
{
    // test 1
    std::cout << daysOfWeek::monday << std::endl;

    // test 2
    std::stringstream ss;
    ss << daysOfWeek::tuesday;
    std::cout << ss.str() << std::endl;

    // test 3
    auto day = enumFactory<daysOfWeek>::toEnum("Wednesday");
    std::cout << day << std::endl;

    system("pause");
}