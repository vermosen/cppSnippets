#pragma once
#ifndef ENUM_MANAGER_HPP
#define ENUM_MANAGER_HPP

#include <array>
#include <iostream>

#include <boost/bimap.hpp>
#include <boost/shared_ptr.hpp>

template <typename T>
struct size;

template <typename T, std::size_t S>
struct create;

template <typename T>
struct key;

template <typename T>
struct pair {
	typedef typename std::pair<typename key<T>::type, T> type;
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
	typedef typename boost::bimap<typename key<T>::type, T> type;
};

template<class T>
struct factory {

private:
	static boost::shared_ptr<typename holder<T>::type> get_map() {
		if (!m_map) {

			m_map.reset(new typename holder<T>::type);
			for (auto it : stash<T>::value) {
				m_map->insert(typename holder<T>::type::value_type(it.first, it.second));
			}
		}

		return m_map;
	}

	~factory() {
		factory<T>::get_map().reset();
	}

public:
	static typename key<T>::type right(T t) { return factory<T>::get_map()->right.at(t); }
	static T left(typename key<T>::type t) { return factory<T>::get_map()->left.at(t); }

private:
	static boost::shared_ptr<typename holder<T>::type> m_map;
};

///////////////////////////////////////////////////////////////////////////////
enum class foo {
	  one   = 1
	, two   = 2
	, three = 3
};

template <>
struct key<foo> {
	typedef typename const char * type;
};

template <> struct create<foo, 0> { static constexpr pair<foo>::type load() { return std::make_pair("one"  , foo::one  ); } };
template <> struct create<foo, 1> { static constexpr pair<foo>::type load() { return std::make_pair("two"  , foo::two  ); } };
template <> struct create<foo, 2> { static constexpr pair<foo>::type load() { return std::make_pair("three", foo::three); } };

template<>
struct size<foo> {
	static constexpr std::size_t value = 3;
};

boost::shared_ptr<typename holder<foo>::type> factory<foo>::m_map;
///////////////////////////////////////////////////////////////////////////////

int main() {

	auto t1 = factory<foo>::right(foo::one);
	auto t2 = factory<foo>::left("one");
	return 0;
}

#endif /* ENUMMANAGER_HPP */