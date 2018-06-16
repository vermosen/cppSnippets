#include<type_traits>
#include <string>
#include <iostream>

#define TEST_IMPL(STR)															\
	template<typename, typename T>												\
	struct has_##STR{															\
		static_assert(															\
			std::integral_constant<T, false>::value,							\
			"Second template parameter needs to be of function type.");			\
	};																			\
																				\
	template<typename C, typename Ret, typename... Args>						\
	struct has_##STR<C, Ret(Args...)> {											\
	private:																	\
		template<typename T>													\
		static constexpr auto check(T*)											\
			-> typename std::is_same<											\
			decltype(std::declval<T>().STR(std::declval<Args>()...)),			\
			Ret																	\
			>::type;															\
																				\
		template<typename>														\
		static constexpr std::false_type check(...);							\
																				\
		typedef decltype(check<C>(0)) type;										\
	public:																		\
		static constexpr bool value = type::value;								\
	};

#define TEST_HAS_METHOD(X, STR, SIGN)												\
	TEST_IMPL(STR)																	\
	static_assert(has_##STR<X, SIGN>::value, "the function has no method "#STR		\
		" with signature " #SIGN "or no implicit conversion exists");				\

namespace foo {
	struct X {
		int serialize(const std::string&, const std::string&) const { return 42; }
	};
}

namespace baz {
	struct Y : foo::X {};
}

TEST_HAS_METHOD(baz::Y, serialize, int(std::string&, std::string&));

int main() {
	return 0;
}
