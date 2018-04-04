#include <iostream>
#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/not.hpp>

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

typedef typename boost::mpl::vector<rate, smallFunction, smallFunction, volatility, volatility>::type vec_type;

template<typename T>
struct is_pdu_impl {
	typedef typename pdu<T> type;
};

template<typename T>
struct is_not_pdu_impl {
	typedef typename boost::mpl::not_<pdu<T>> type;
};

template <typename T>
struct is_pdu {
	typedef typename boost::mpl::copy_if<
		T, is_pdu_impl<boost::mpl::_>::type
		, boost::mpl::back_inserter<boost::mpl::vector<>>
	>::type type;
};

template <typename T>
struct is_not_pdu {
	typedef typename boost::mpl::copy_if<
		T, boost::mpl::not_<is_pdu_impl<boost::mpl::_>::type>
		, boost::mpl::back_inserter<boost::mpl::vector<>>
	>::type type;
};

typedef typename is_pdu<vec_type>::type pdus;
typedef typename is_not_pdu<vec_type>::type non_pdus;

int main() {
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<pdus>::value, == , 3);
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<non_pdus>::value, ==, 2);
	BOOST_MPL_ASSERT((std::is_same< boost::mpl::at_c<pdus, 0>::type, rate>));
	pdus t;
	return 0;
}
