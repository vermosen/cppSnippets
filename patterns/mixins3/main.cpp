#include <iostream>

template <typename T>
struct mixin1 {};

template <typename T>
struct mixin2 {};

template <
      template <template <class> class ...> class base
    , template <class> class ... Ts>
struct curiousMixin : public Ts<base<Ts...>>... {

    template <template <class> class T>
    T<base<Ts...>>& impl() {
        return static_cast<T<base<Ts...>>&>(*this);
    }
};

template <template <class> class ... Ts>
struct foo : public curiousMixin<foo, Ts...> {

};

int main()
{
    using base = foo<mixin1, mixin2>;
    using type_1 = mixin1<base>;
    using type_2 = mixin2<base>;

    base b;
    type_1& t1 = b.template impl<mixin1>();
    type_2& t2 = b.template impl<mixin2>();

    return 0;
}
