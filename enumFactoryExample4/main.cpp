#pragma once
#ifndef ENUM_MANAGER_HPP
#define ENUM_MANAGER_HPP

#include <array>
#include <iostream>
#include <functional>

//template <typename _enum>
//struct size;
//
//template <typename _enum>
//struct factory;
//

//
//template <>
//struct size<foo> {
//	static constexpr std::size_t value = 2;
//};
//
//template <>
//struct factory<foo> {
//public:
//	static constexpr std::array<foo, size<foo>::value> m_array = {};
//};
//
//template<typename _enum, int I>
//struct load {
//	constexpr void step() {
//		std::get<0>(factory<foo>::m_array) = _enum
//	}
//};

//int main() {
//
//	auto tt = factory<foo>::m_array;
//
//	return 0;
//}

enum class foo {
	  one   = 1
	, two   = 2
	, three = 3
};

template <int S>
struct create {
	static void LoadingStep() {}
};

template <> struct create<0> { static constexpr foo load() { return foo::one; } };
template <> struct create<1> { static constexpr foo load() { return foo::two; } };
template <> struct create<2> { static constexpr foo load() { return foo::three; } };

template<template<int S> class T, int N, int... Rest>
struct array_impl {
	static constexpr auto& value = array_impl<T, N - 1, N, Rest...>::value;
};

template<template<int S> class T, int... Rest>
struct array_impl<T, 0, Rest...> {
	static constexpr std::array<foo, sizeof...(Rest) + 1> value = {
		  T<0>::load(), T<Rest>::load()...};
};

template<template<int S> class T, int... Rest>
constexpr std::array<foo, sizeof...(Rest) + 1> array_impl<T, 0, Rest...>::value;

template<template<int S> class T, int N>
struct factory {
	static_assert(N >= 0, "N must be at least 0");
	static constexpr auto& value = array_impl<T, N - 1>::value;
	factory() = delete;
	factory(const factory&) = delete;
	factory(factory&&) = delete;
};

int main() {
	auto& value = factory<create, 3>::value;
	return 0;
}

#endif /* ENUMMANAGER_HPP */