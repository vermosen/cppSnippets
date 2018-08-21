
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/size.hpp>

#include <tuple>

struct a {};
struct b {};
struct c {};
struct d {};
struct e {};

struct _1 {};
struct _2 {};
struct _3 {};

template <typename T>
struct trait;

template <>
struct trait<a> {
    typedef _1 type;
};

template <>
struct trait<b> {
    typedef _2 type;
};

template <>
struct trait<c> {
    typedef _1 type;
};

template <>
struct trait<d> {
    typedef _2 type;
};

template <>
struct trait<e> {
    typedef _1 type;
};

namespace pl = boost::mpl::placeholders;

template <typename ... Args>
struct folder {
public:
    typedef typename boost::mpl::fold<
        typename boost::mpl::vector<Args...>::type
        , boost::mpl::set0<>
        , boost::mpl::insert<pl::_1, pl::_2>
    >::type type;
};

template <template <class> class F, typename ... Args>
struct aggregator {
private:
    template <typename ... Args>
    struct folder {
    public:
        typedef typename boost::mpl::fold<
            typename boost::mpl::vector<Args...>::type
            , boost::mpl::set0<>
            , boost::mpl::insert<pl::_1, pl::_2>
        >::type type;
    };

public:
    typedef typename folder<typename F<Args>::type...>::type type;
};

int main() {

    // create a boost::mpl::set of the desired types
    typedef aggregator<trait, a, b, c, d, e>::type res;

    // output the type
    res a;
    return EXIT_SUCCESS;
}
