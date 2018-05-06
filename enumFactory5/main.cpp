#pragma once
#ifndef ENUM_MANAGER_HPP
#define ENUM_MANAGER_HPP

#include <array>
#include <iostream>
#include <functional>

namespace test {

	template <typename T>
	struct size;

	enum class foo {
		one = 1
		, two = 2
		, three = 3
	};

	template<>
	struct size<foo> {
		static constexpr std::size_t value = 3;
	};

	template <int S>
	struct create {
		static void load() {}
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
		static constexpr std::array<foo, sizeof...(Rest)+1> value = {
			  T<0>::load(), T<Rest>::load()... };
	};

	template<template<int S> class T, int... Rest>
	constexpr std::array<foo, sizeof...(Rest)+1> array_impl<T, 0, Rest...>::value;

	template<template<int S> class T>
	struct factory {
		static_assert(size<foo>::value >= 0, "N must be at least 0");
		static constexpr auto& value = array_impl<T, size<foo>::value - 1>::value;
		factory() = delete;
		factory(const factory&) = delete;
		factory(factory&&) = delete;
	};
}

int main() {
	auto& value = test::factory<test::create>::value;
	return 0;
} 

#endif /* ENUMMANAGER_HPP */