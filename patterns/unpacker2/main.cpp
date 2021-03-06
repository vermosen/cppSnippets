#pragma once

#include <string>
#include <iostream>

enum class type {
      a = 1
    , b = 2
    , c = 3
    , none = 0
};

template <type T>
class item;

template <template <type T> class Functor, type ... Type>
struct unpacker;

template <template <type T> class Functor, type First, type Second, type ... Rest>
struct unpacker<Functor, First, Second, Rest...> : public unpacker<Functor, Second, Rest...> {
    type apply(const std::string& arg) {
        if (Functor<First>::apply(arg)) {
            return unpacker<Functor, Second, Rest...>::apply(arg);
        }
        else {
            return First;
        }
    }
};

template <template <type T> class Functor, type First, type Second>
struct unpacker<Functor, First, Second> : public unpacker<Functor, Second> {
    type apply(const std::string& arg) {
        if (Functor<First>::apply(arg)) {
            return unpacker<Functor, Second>::apply(arg);
        }
        else {
            return First;
        }
    }
};

template <template <type T> class Functor, type First>
struct unpacker<Functor, First> {
    type apply(const std::string& arg) {
        if (Functor<First>::apply(arg)) {
            return type::none;
        } else {
            return First;
        }
    }
};

template <>
class item<type::a> {
public:
    static bool apply(const std::string& s) { return s != "a"; }
};

template <>
class item<type::b> {
public:
    static bool apply(const std::string& s) { return s != "b"; }
};

template <>
class item<type::c> {
public:
    static bool apply(const std::string& s) { return s != "c"; }
};

int main() {
    unpacker<item, type::a, type::b, type::c> up;
    auto res = up.apply("c");
    return 0;
}
