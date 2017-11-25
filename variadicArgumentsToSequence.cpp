#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/assert.hpp>
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/equal.hpp"

#include <iostream>

template <typename... Elems>
class impl;

template <typename Elem, typename... Elems>
class impl<Elem, Elems...> : public impl<Elems...>
{
public:
    typedef typename boost::mpl::push_back<typename impl<Elems...>::sequence, Elem>::type sequence;

    impl() : impl<Elems...>() {}
    virtual ~impl() {}
};

template <typename Elem>
class impl<Elem>
{
public:
    typedef typename boost::mpl::vector<Elem> sequence;

    impl() {}
    virtual ~impl() {}
};

int main()
{
    BOOST_MPL_ASSERT((
        boost::mpl::equal<impl<double, bool, int>::sequence,
        boost::mpl::vector3<int, bool, double>>));

    return 0;
}
