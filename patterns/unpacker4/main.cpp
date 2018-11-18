#pragma once

#include <string>
#include <iostream>

namespace traits {
    template <typename F>
    struct argType;

    template <typename F>
    struct valueType;

    template <typename R, typename T>
    struct argType<R(*)(T)> {
        typedef T type;
    };

    template <typename R, typename T>
    struct valueType<R(*)(T)> {
        typedef R type;
    };
}


///////////////////////////////////

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
struct unpacker<Functor, First, Second, Rest...> : public item<First>, public unpacker<Functor, Second, Rest...> {

    using item<First>::init;
    using unpacker<Functor, Second, Rest...>::init;
    
    using arg_type = typename traits::argType<decltype(&item<First>::item)>::type;


    unpacker() : item<First>

    type apply(const std::string& arg) {
        if (item<First>::apply(arg)) {
            return unpacker<Functor, Second, Rest...>::apply(arg);
        }
        else {
            return First;
        }
    }
};

template <template <type T> class Functor, type First, type Second>
struct unpacker<Functor, First, Second> : public item<First>, public unpacker<Functor, Second> {

    using item<First>::init;
    using unpacker<Functor, Second>::init;

    type apply(const std::string& arg) {
        if (item<First>::apply(arg)) {
            return unpacker<Functor, Second>::apply(arg);
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
    item(int i) : m_i(i) {}
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
