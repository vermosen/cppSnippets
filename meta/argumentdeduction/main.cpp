#pragma once

#include <type_traits>

template<class T>
struct constructible {
public:
    template<class...Args>
    struct from {
        template<class C>
        static C arg() {};
    public:
        static constexpr bool value = std::is_same<T*, decltype(T(arg<Args>()...))*>::value;
    };
};

struct a
{
    a() {}
    a(int i) {}
    a(bool& i) {}
};


int main() {
    static_assert(
        constructible<a>::from<>::value,
        "a()"
        );

    static_assert(
        constructible<a>::from<int>::value,
        "a(int i)"
        );

    static_assert(
        constructible<a>::from<int&>::value,
        "a(int i)"
        );

    /*
    static_assert(
        constructible<a>::from<bool*>::value,       // fails
        "a(bool i)"
        );
    */
}
