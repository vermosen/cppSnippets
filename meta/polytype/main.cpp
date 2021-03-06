
#include <iostream>

// type 1
//class base1 {
//    virtual void foo() {};
//};
//
//class derived1 : public base1 {
//    virtual void foo() final override {};
//};
//
//// type 2
//template <typename T>
//class base2 {
//    void foo() {
//        return static_cast<T&>(*this).fooImpl();
//    };
//};
//
//class derived2 : public base2<derived2> {
//    void fooImpl() {};
//};

enum class polymorphism {
      compilation
    , runtime
};

template <polymorphism Type, class Derived = void>
class base;

template <template <polymorphism, class> class Base>
class derived;

template <class Derived>
class base<polymorphism::runtime, Derived> {

};

int main()
{
    //base1* b1 = new derived1;
    //base2<derived2>& b2(derived2());
    return 0;
}
