#include  <iostream>

#include <boost/mpl/vector.hpp>

template <typename ... Types>
struct amongst {
    using type = boost::mpl::vector<Types...>::type;
};

template<typename ... Types>
class visitor;

template<typename T>
class visitor<T> {
public:
    virtual void apply(T & visitable) = 0;
};

template<typename First, typename... Others>
class visitor<First, Others...> : public visitor<Others...> {
public:
    using visitor<Others...>::accept;

    virtual void apply(First& visitable) = 0;
};

namespace details {
    template<typename ... Types>
    class visitable {
    public:
        virtual void accept(visitor<Types...>& visitor) = 0;
    };
}

template<typename Base, typename Pack>
class visitable : public Base, public details::visitable<Pack> {
public:
    virtual void accept(visitor<Types...>& visitor) {
        visitor.visit(static_cast<Derived&>(*this));
    }
};

// example
class base {
    virtual void foo() { std::cout << "hw (base)" << std::endl; }
};

class Foo; class Bar; class Baz;

class Foo : public visitable<base, amongst<Foo, Bar, Baz>> {
    virtual void foo() override final { std::cout << "hw (foo)" << std::endl; }
};
