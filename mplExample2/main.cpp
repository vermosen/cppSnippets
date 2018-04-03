#include <iostream>

#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal.hpp>

typedef boost::mpl::vector<void> entities;

template<typename T>
struct pusher {

    typedef typename boost::mpl::push_back<entities, T>::type types;
};
    
int main() {

    pusher<int> p1;
    pusher<double> p2;

    BOOST_MPL_ASSERT((
    mpl::equal<
        entities,
    mpl::vector<void, int, double>
    >));
        
    return 0;
}
