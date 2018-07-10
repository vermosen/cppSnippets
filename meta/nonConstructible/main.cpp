
#include <type_traits>

template <typename T>
struct nonDefaultConstructible {

	nonDefaultConstructible() {
		static_assert(!std::is_default_constructible<T>::value);
	}
};

struct test : public nonDefaultConstructible<test> {

	test(int a) : m_a() {}

	int m_a;
};

int main() {

	auto t = test(1);
	return 0;
}
