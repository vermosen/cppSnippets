#include <iostream>

template <typename T>
struct mixin1 {};

template <typename T>
struct mixin2 {};

template <template <class> class ... Ts>
struct bag : public Ts<bag<Ts...>>... {};

int main()
{
    using base = bag<mixin1, mixin2>;
    using type_1 = mixin1<base>;
    using type_2 = mixin2<base>;

    base b;
    type_1& t1 = static_cast<type_1&>(b);
    type_2& t2 = static_cast<type_2&>(b);

    return 0;
}
