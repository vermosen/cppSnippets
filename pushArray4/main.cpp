#include <array>
#include <iostream>

template<unsigned Sz>
struct array_holder_impl;

template<> 
struct array_holder_impl<0> {
	static constexpr std::array<unsigned, 0> data = {};
};

template<unsigned Sz>
constexpr std::array<unsigned, Sz> push_one(unsigned arg) {

	static std::array<unsigned, Sz> _new = {};

	for (int i = 0; i < array_holder_impl<Sz - 1>::data.size(); i++) {
		_new[i] = array_holder_impl<Sz - 1>::data[i]; //	require c++ 14
	}

	_new[Sz] = arg;
	return std::move(_new);
}

template<>
struct array_holder_impl<1> {
	static constexpr std::array<unsigned, 1> data = push_one<1>(6);
};

int main() {
	auto res = array_holder_impl<1>::data;

	return 0;
}