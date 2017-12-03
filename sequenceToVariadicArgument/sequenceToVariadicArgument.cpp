#include <iostream>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>

#include <boost/mpl/assert.hpp>

class A {};
class B {};
class C {};
class D {};

template <typename T, typename... Feeds>
class target;

template < class T, class R >
struct foo;

template < typename T, class... TTypes, class X >
struct foo< target< typename T, TTypes... >, X >
{
    typedef target< typename T, TTypes..., X > type;
};

typedef boost::mpl::vector<B, C, D> types;

typedef boost::mpl::fold< types, target<A>,
    foo<boost::mpl::_1, boost::mpl::_2> >::type result_normalized;

int main() {
    BOOST_MPL_ASSERT((std::is_same<result_normalized, target<A, B, C, D> >));
    result_normalized t();
    return 0;
}
