#pragma once

#include <string>
#include <iostream>

enum class type {
      a     = 1
    , b     = 2
    , c     = 3
    , none  = 0
};

template <type T>
class item;

template <template <type T> class Functor, type ... Type>
struct unpacker;

template <template <type T> class Functor, type First, type ... Rest>
struct unpacker<Functor, First, Rest...> : public item<First>, public unpacker<Functor, Rest...> {

    using item<First>::init;
    using unpacker<Functor, Rest...>::init;

    type apply(const std::string& arg) {
        if (item<First>::apply(arg)) {
            return unpacker<Functor, Rest...>::apply(arg);
        }
        else {
            return First;
        }
    }
};

template <template <type T> class Functor, type First>
struct unpacker<Functor, First> : public item<First> {

    using item<First>::init;

    type apply(const std::string& arg) {
        if (item<First>::apply(arg)) {
            return type::none;
        }
        else {
            return First;
        }
    }
};

template <>
class item<type::a> {
public:
    void init(int i) { m_i = i; }
    bool apply(const std::string& s) { return s != "a"; }

protected:
    int m_i;
};

template <>
class item<type::b> {
public:
    void init() {}
    bool apply(const std::string& s) { return s != "b"; }
};

template <>
class item<type::c> {
public:
    void init() {}
    bool apply(const std::string& s) { return s != "c"; }
};

int main() {
    unpacker<item, type::a, type::b, type::c> up;
    up.item<type::a>::init(3);
    up.item<type::b>::init();
    up.item<type::c>::init();
    auto res = up.apply("c");
    return 0;
}
