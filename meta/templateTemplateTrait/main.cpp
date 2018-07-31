#include <iostream>

template <class K>
struct foo {
	K m_m = 0;
};

template <template <class> class T>
struct trait {

	template <class K>
	using type = T<K>;
};

template <typename T>
struct alias {
	template <class K>
	using type = typename T::template type<K>;
};

int main() {
	typename alias<trait<foo>>::type<double> f;
	std::cout << f.m_m << std::endl;
	return 0;
}
