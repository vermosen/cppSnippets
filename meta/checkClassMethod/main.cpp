#include<type_traits>

class test {

	size_t used_memory() const { return 1; }
};

template<typename T>
struct hasUsedMemoryMethod
{
	template<typename U, size_t(U::*)() const> struct SFINAE {};
	template<typename U> static char attempt(SFINAE<U, &U::used_memory>*);
	template<typename U> static int attempt(...);
	static constexpr bool hasMethod = sizeof(attempt<T>(nullptr)) == sizeof(char);
	static_assert(hasMethod, "no function provided");

};

int main() {

	hasUsedMemoryMethod<test> t;
	auto ttt = hasUsedMemoryMethod<test>::hasMethod;
	return 0;
}
