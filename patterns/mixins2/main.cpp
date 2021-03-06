#include <iostream>
#include <tuple>

#include "fwd/curiousmixins.h"

template <
      class Base
    , template <class> class ... Ts
>
struct curiousMixins : public Ts<Base>... {

    template <typename ... Xs>
    curiousMixins(Xs&& ... xs) : Ts<Base>(std::forward<Xs>(xs))... {}

    template <template <class> class T>
    T<Base>& impl() {
        return static_cast<T<Base>&>(*this);
    }
};

template <class T>
struct mixin1 {
    mixin1(const std::tuple<int, double>& t)
        : m_x(std::get<0>(t))
        , m_y(std::get<1>(t)) {}
    void foo() { std::cout << m_x << ", " << m_y; }

protected:
    int m_x;
    int m_y;
};

template <class T>
struct mixin2 {
    mixin2(double z) : m_z(z) {}
    void foo() { std::cout << m_z; }
protected:
    double m_z;
};

struct foo : public curiousMixins<foo, mixin1, mixin2> {
    using ancestor = curiousMixins<foo, ::mixin1, ::mixin2>;

    foo(const std::tuple<int, double>& t, double k) : ancestor(t, k) {}

    void print() {

        std::cout << "[";
        impl<::mixin1>().foo();
        std::cout << ", ";
        impl<::mixin2>().foo();
        std::cout << "]" << std::endl;
    }
};

int main()
{
    using base = foo;
    using type_1 = mixin1<base>;
    using type_2 = mixin2<base>;

    base b(std::tuple<int, double>(1, 2.0), 3.0);
    b.print();
    return 0;
}
