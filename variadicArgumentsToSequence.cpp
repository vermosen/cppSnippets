#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/optional.hpp>

#include <iostream>

struct impl_tag {};

template <typename... Elems>
struct impl : public impl_tag {};

template <typename Elem, typename... Elems>
struct impl<Elem, Elems...> : public impl<Elems...>
{
public:
    typedef typename boost::mpl::push_back<typename impl<Elems...>::sequence, Elem>::type sequence;

    //// we do not allow for nested structure
    //BOOST_MPL_ASSERT_NOT((std::is_base_of<impl_tag, Elem>));

    //// no optional either
    //BOOST_MPL_ASSERT_NOT((std::is_base_of<boost::optional_detail::optional_tag,
    //    typename std::remove_reference<Elem>>));

    impl() : impl<Elems...>()
    {
        static_assert((std::is_same<std::is_base_of<impl_tag, Elem>::type, std::false_type>::value), "");
        static_assert((std::is_same<std::is_base_of<boost::optional_detail::optional_tag,
            Elem>::type, std::false_type>::value), "");

        static_assert((std::is_same<std::is_base_of<boost::optional_detail::optional_tag,
            std::remove_reference<Elem>::type>::type, std::false_type>::value), "");
    }
    virtual ~impl() {}
};

template <typename Elem>
struct impl<Elem> : public impl_tag
{
public:
    typedef typename boost::mpl::vector<Elem> sequence;

    impl() : impl_tag()
    {
        static_assert((std::is_same<std::is_base_of<impl_tag, Elem>::type, std::false_type>::value), "");
        static_assert((std::is_same<std::is_base_of<boost::optional_detail::optional_tag,
            Elem>::type, std::false_type>::value), "");

        static_assert((std::is_same<std::is_base_of<boost::optional_detail::optional_tag,
            std::remove_reference<Elem>::type>::type, std::false_type>::value), "");
    }
    virtual ~impl() {}
};

int main()
{
    BOOST_MPL_ASSERT((
        boost::mpl::equal<impl<double, bool, int>::sequence,
        boost::mpl::vector3<int, bool, double>>));

    impl<double, bool&, int> t;

    return 0;
}
