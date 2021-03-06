#include <iostream>

namespace tags {
    class yes {};
    class no {};
}

template <bool Bool>
struct sfinae {};

template <>
struct sfinae <true> {
    void foo() { std::cout << "hello world!" << std::endl; }
};


struct test1 : public sfinae<true> {

};

struct test2 : public sfinae<false> {

};


int main()
{
    test1 t1;
    test2 t2;
    t1.foo();

    // compilation fails !!
    // t2.foo();

    return 0;
}

