#include <iostream>

enum class test : char {
      one = 1
    , two = 2
};

template <test T>
struct printer;

template <>
struct printer <test::one> {
    static constexpr const char out[] = "ONE";
};

template <>
struct printer <test::two> {
    static constexpr const char out[] = "TWO";
};

template <const char* Str>
struct X {
    const char* GetString() const
    {
        return Str;
    }
};


int main()
{
    static constexpr char global_string[] = "String";
    X<printer<test::two>::out> x;
    std::cout << x.GetString();
    return 0;
}

//template <test T>
//struct printer;
//
//template <>
//struct printer <test::one> {
//    static constexpr const char* out = "ONE";
//};
//
//template <>
//struct printer <test::two> {
//    static constexpr const char* out = "TWO";
//};
//
//static constexpr in_map
//
//int main() {
//    std::cout << printer<test::two>::out << std::endl;
//    return 0;
//}
