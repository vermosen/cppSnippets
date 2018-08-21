#include <iostream>

// constexpr size of a char
constexpr unsigned size(char const* str, unsigned count = 0){
    return ('\0' == str[0]) ? count : size(str + 1, count + 1);
}

namespace details {
    template <char First, char... Rest>
    struct printer {
        static void apply() {
            std::cout << First;
            printer<Rest...>::apply();
        }
    };

    template <char First>
    struct printer<First> {
        static void apply() { std::cout << First; }
    };
}

// destination "template string" type
template < char... Args >
struct printer {
    static void apply() {
        details::printer<Args...>::apply();
    }
};

// struct to explode a `char const*` to an `exploded_string` type
template <typename Provider, std::size_t Len, char... Args>
struct explode_impl
{
    using result =
    typename explode_impl <
          Provider
        , Len - 1
        , Provider::value()[Len - 1]
        , Args...
    >::result;
};

template <typename Provider, char... Args>
struct explode_impl <Provider, 0, Args...> {
    using result = printer<Args...>;
};

// syntactical sugar
template <typename Provider>
using const_str =
typename explode_impl <
      Provider
    , size(Provider::value())
>::result;

int main()
{
    struct str1 {
        constexpr static char const* value() { return "hello"; }
    };

    struct str2 {
        constexpr static char const* value() { return "world"; }
    };

    auto my_str1 = const_str<str1>{};    // as a variable
    using My_Str1 = const_str<str1>;    // as a type
    my_str1.apply();

    auto my_str2 = const_str<str2>{};    // as a variable
    using My_Str2 = const_str<str2>;    // as a type
    my_str2.apply();
    return 0;
}
