#pragma once
#ifndef ENUM_MANAGER_HPP
#define ENUM_MANAGER_HPP

#include <array>
#include <iostream>

#include <boost/bimap.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>

// Doubles the parentheses of a Boost.PP sequence, i.e. (a, b)(c, d) becomes ((a, b))((c, d))
#define CX_SEQ_DOUBLE_PARENS_0(...) \
     ((__VA_ARGS__)) CX_SEQ_DOUBLE_PARENS_1

#define CX_SEQ_DOUBLE_PARENS_1(...) \
     ((__VA_ARGS__)) CX_SEQ_DOUBLE_PARENS_0

#define CX_SEQ_DOUBLE_PARENS_0_END
#define CX_SEQ_DOUBLE_PARENS_1_END

#define CX_SEQ_DOUBLE_PARENS(seq) \
    BOOST_PP_CAT(CX_SEQ_DOUBLE_PARENS_0 seq, _END)

namespace utils {
namespace details {

	template <typename T>
	struct size;

	template <typename T, std::size_t S>
	struct create;

	template <typename T>
	struct key_t;

	template <typename T>
	struct pair {
		typedef typename std::pair<typename key_t<T>::type, T> type;
	};

	template<class T, std::size_t N, std::size_t... Rest>
	struct array_impl {
		static constexpr auto& value = array_impl<T, N - 1, N, Rest...>::value;
	};

	template<class T, std::size_t... Rest>
	struct array_impl<T, 0, Rest...> {
		static constexpr std::array<typename pair<T>::type, sizeof...(Rest)+1> value = {
			create<T, 0>::load(), create<T, Rest>::load()...
		};
	};

	template<class T, std::size_t... Rest>
	constexpr std::array<typename pair<T>::type, sizeof...(Rest)+1> array_impl<T, 0, Rest...>::value;

	template<class T>
	struct stash {
		static_assert(size<T>::value >= 1, "N must be at least 1");

		stash() = delete;
		stash(const stash&) = delete;
		stash(stash&&) = delete;

		static constexpr auto& value = array_impl<T, size<T>::value - 1>::value;
	};

	template<typename T>
	struct holder {
		typedef typename boost::bimap<typename key_t<T>::type, T> type;
	};
}}

namespace utils {
	template<class T>
	struct factory {
	private:
		static boost::shared_ptr<typename details::holder<T>::type> get_map() {
			if (!m_map) {
				m_map.reset(new typename details::holder<T>::type);
				for (auto it : details::stash<T>::value) {
					m_map->insert(typename details::holder<T>::type::value_type(it.first, it.second));
				}
			}

			return m_map;
		}

		~factory() {
			factory<T>::get_map().reset();
		}

	public:
		static typename details::key_t<T>::type key(T t) { return factory<T>::get_map()->right.at(t); }
		static T value(typename details::key_t<T>::type t) { return factory<T>::get_map()->left.at(t); }

	private:
		static boost::shared_ptr<typename details::holder<T>::type> m_map;
	};
}

#define DEFINE_ENUM(X) \
boost::shared_ptr<typename utils::details::holder<X>::type> utils::factory<X>::m_map;

#define DECLARE_ELEM(r, data, elem) \
    BOOST_PP_TUPLE_ELEM(3, 0, elem) = BOOST_PP_TUPLE_ELEM(3, 1, elem) ,

#define FOR_EACH_DECLARE_ENUM(SEQ) \
	BOOST_PP_SEQ_FOR_EACH(DECLARE_ELEM, ~, CX_SEQ_DOUBLE_PARENS(SEQ))

#define DECLARE_CREATOR(r, X, I, elem)								\
	template <> struct utils::details::create<X, I> {				\
		static constexpr utils::details::pair<X>::type load() {		\
			return std::make_pair(BOOST_PP_TUPLE_ELEM(3, 2, elem),	\
			X::BOOST_PP_TUPLE_ELEM(3, 0, elem)  );					\
		}															\
	};

#define FOR_EACH_DECLARE_CREATOR(X, SEQ) \
	BOOST_PP_SEQ_FOR_EACH_I(DECLARE_CREATOR, X, CX_SEQ_DOUBLE_PARENS(SEQ))

#define DECLARE_ENUM_ALL(X, Y, T, SEQ)		\
enum class X : Y {							\
	FOR_EACH_DECLARE_ENUM(SEQ)				\
	none = 0								\
};											\
											\
template <>									\
struct utils::details::key_t<X> {			\
	typedef typename T type;				\
};											\
											\
template<>									\
struct utils::details::size<X> {			\
	static constexpr std::size_t value =	\
	BOOST_PP_SEQ_SIZE(SEQ);					\
};											\
FOR_EACH_DECLARE_CREATOR(X, SEQ)			\
											\
template <typename T>						\
inline T& operator<<(T& os, X elem) {		\
	os << utils::factory<X>::key(elem);		\
	return (os);							\
}

#define DECLARE_ENUM(X, SEQ)				\
	DECLARE_ENUM_ALL(X, int, const char*, SEQ)

DECLARE_ENUM(foo,
	(one, 1, "one")
	(five, 5, "five")
	(three, 3, "three")
)

// cpp
DEFINE_ENUM(foo);

int main() {
	std::cout << utils::factory<foo>::key(foo::one) << std::endl;
	auto t2 = utils::factory<foo>::value("five");
	return 0;
}

#endif /* ENUMMANAGER_HPP */