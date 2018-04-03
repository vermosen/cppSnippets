#include <iostream>
#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/less.hpp>

struct rate {};
struct smallFunction {};
struct volatility {};

template <typename T>
struct pdu;

template <>
struct pdu<rate> {
	typedef typename boost::mpl::bool_<true> type;
};

template <>
struct pdu<smallFunction> {
	typedef typename boost::mpl::bool_<false> type;
};

template <>
struct pdu<volatility> {
	typedef typename boost::mpl::bool_<true> type;
};

typedef typename boost::mpl::vector<rate, smallFunction, volatility>::type vec_type;

template<typename T>
struct is_pdu {
	typedef typename pdu<T> type;
};

//template <typename T>
//struct filter {
//	typedef typename boost::mpl::transform_view<
//		boost::mpl::filter_view<T, is_pdu<boost::mpl::_> >,
//		boost::mpl::sizeof_<boost::mpl::_>
//	>::type type;
//};

template <typename T>
struct filter {
	typedef typename boost::mpl::copy_if<
		T, is_pdu<boost::mpl::_>::type
		, boost::mpl::back_inserter<boost::mpl::vector<>>
	>::type type;
};

typedef typename filter<vec_type>::type pdus;

int main() {

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<pdus>::value, ==, 2);
	BOOST_MPL_ASSERT((std::is_same< boost::mpl::at_c<pdus, 0>::type, rate>));
	pdus t;
	return 0;
}
