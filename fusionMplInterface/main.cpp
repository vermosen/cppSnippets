
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/mpl.hpp> // Required to adapt Fusion to MPL
#include <boost/mpl/transform.hpp>

template <typename T>
struct wrapper
{
    typedef T type;
};


template <typename T>
struct unwrap
{
    typedef typename T::type type;
};

typedef boost::fusion::vector<wrapper<int>, wrapper<double>, wrapper<double>> source;
typedef boost::mpl::transform<source, unwrap<boost::mpl::_> >::type sequence;
typedef boost::fusion::result_of::as_vector<sequence>::type dest;

int main()
{
    dest d(1, 2.3, 2.0);
    return 0;
}
