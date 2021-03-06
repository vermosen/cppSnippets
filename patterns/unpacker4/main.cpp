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

template <type ... Type>
struct unpacker;

template <type First, type ... Rest>
struct unpacker<First, Rest...> : public item<First>, public unpacker<Rest...> {

    unpacker(
          const typename item<First   >::pack& first
        , const typename item<Rest>::pack&... rest...)
        : unpacker<Rest...>(rest...)
        , item<First>(first) {}

    type apply(const std::string& arg) {
        if (item<First>::apply(arg)) {
            return unpacker<Rest...>::apply(arg);
        }
        else {
            return First;
        }
    }
};

template <type First>
struct unpacker<First> : public item<First> {

    unpacker(const typename item<First>::pack& first)
        : item<First>(first) {}

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
    struct pack { int m_i; };
    item(const pack&) {}
    void init(int i) { m_i = i; }
    bool apply(const std::string& s) { return s != "a"; }

protected:
    int m_i;
};

template <>
class item<type::b> {
public:
    struct pack { double m_d; };
    item(const pack&) {}
    void init(double d) { d = m_d; }
    bool apply(const std::string& s) { return s != "b"; }
protected:
    double m_d;
};

template <>
class item<type::c> {
public:
    struct pack {};
    item(const pack&) {}
    bool apply(const std::string& s) { return s != "c"; }
};

int main() {
    item<type::a>::pack p_a;
    item<type::b>::pack p_b;
    item<type::c>::pack p_c;

    unpacker<type::a, type::b, type::c> up(p_a, p_b, p_c);
    //up.item<type::a>::init(3);
    //up.item<type::b>::init(2.0);
    //up.item<type::c>::init();
    auto res = up.apply("c");
    return 0;
}
