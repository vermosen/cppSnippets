#include <array>
#include <iostream>

template<unsigned... args> struct ArrayHolder {
	static constexpr std::array<unsigned, sizeof...(args)> data = { args... };
};

template<size_t N, template<size_t> class F, unsigned... args>
struct generate_array_impl {
	typedef typename generate_array_impl<N - 1, F, F<N>::value, args...>::result result;
};

template<template<size_t> class F, unsigned... args>
struct generate_array_impl<0, F, args...> {
	typedef ArrayHolder<F<0>::value, args...> result;
};

template<size_t N, template<size_t> class F>
struct generate_array {
	typedef typename generate_array_impl<N - 1, F>::result result;
};

template<size_t index> struct MetaFunc {
	enum { value = index + 1 };
};

int main() {
	const size_t count = 5;
	typedef generate_array<count, MetaFunc>::result A;

	for (size_t i = 0; i<count; ++i)
		std::cout << A::data[i] << "\n";

	return 0;
}