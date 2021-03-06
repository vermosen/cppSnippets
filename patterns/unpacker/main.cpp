#pragma once

#include <iostream>

enum class type {
      a = 1
    , b = 2
    , c = 3
};

template <type Type>
struct atom;

template <type ... Types>
struct unpacker : public atom<Types>... {

    template<type T>
    void apply() { return atom<T>::apply(); };
};

template <>
struct atom<type::a> {
    void apply() { std::cout << "a" << std::endl; }
};

template <>
struct atom<type::b> {
    void apply() { std::cout << "b" << std::endl; }
};

template <>
struct atom<type::c> {
    void apply() { std::cout << "c" << std::endl; }
};

int main()
{
    unpacker<type::a, type::b, type::c> up;
    up.template apply<type::a>();
    up.template apply<type::b>();
    up.template apply<type::c>();
    return 0;
}
