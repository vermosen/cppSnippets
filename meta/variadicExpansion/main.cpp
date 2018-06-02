#include <iostream>

struct A {};
struct B {};
struct C {};

enum class type {

	  A = 1
	, B = 2
	, C = 3
};

template<typename A>
struct checker;

template<>
struct checker<A> {
	inline static void impl() { std::cout << "a"; }
};

template<>
struct checker<B> {
	inline static void impl() { std::cout << "b"; }
};

template<>
struct checker<C> {
	inline static void impl() { std::cout << "c"; }
};


template <class First, class ... Rest>
struct expanse;

template<class First, class ... Rest>
struct interface : public expanse<First, Rest...> {

	using  base = expanse<First, Rest...>;

	inline void checkAll() {
		base::checkAll();
	}

	template<typename T>
	void checkOne() { 
		checker<T>::impl(); 
		std::cout << std::endl;
	}
};

template <class First, class ... Rest>
struct expanse : public expanse<Rest...> {

	using  base = expanse<Rest...>;

	inline void checkAll() {
		checker<First>::impl();
		std::cout << " ";
		base::checkAll();
	}
};

template <class First>
struct expanse<First> {

	inline void checkAll() { 
		checker<First>::impl();
		std::cout << "!" << std::endl;
	}
};

int main() {

	interface<A, B, C> test;

	// recursive method
	test.checkAll();

	test.checkOne<A>();
	test.checkOne<B>();
	test.checkOne<C>();

	return 0;

}