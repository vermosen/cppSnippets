#include<type_traits>
#include <string>
#include <iostream>

template<typename, typename T>
struct has_serialize {
	static_assert(
		std::integral_constant<T, false>::value,
		"Second template parameter needs to be of function type.");
};

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> {
private:
	template<typename T>
	static constexpr auto check(T*)
		-> typename
		std::is_same<
		decltype(std::declval<T>().serialize(std::declval<Args>()...)),
		Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		>::type;  // attempt to call it and see if the return type is correct

	template<typename>
	static constexpr std::false_type check(...);

	typedef decltype(check<C>(0)) type;

public:
	static constexpr bool value = type::value;
};

struct X {
	int serializeee(const std::string&) { return 42; }
};

struct Y : X {};

int main() {
	std::cout << has_serialize<Y, int(const std::string&)>::value; // will print 1
	return 0;
}
