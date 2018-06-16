#include <iostream>

struct A {};
struct B {};
struct C {};

template<typename Caller, typename A>
struct checker;

template<typename Caller>
struct checker<Caller, A> {
	inline static void impl(Caller& c) { std::cout << "a"; }
};

template<typename Caller>
struct checker<Caller, B> {
	inline static void impl(Caller& c) { std::cout << "b"; }
};

template<typename Caller>
struct checker<Caller, C> {
	inline static void impl(Caller& c) { std::cout << "c"; }
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
		checker<expanse<First, Rest...>, T>::impl(*this);
		std::cout << std::endl;
	}
};

template <class First, class ... Rest>
struct expanse : public expanse<Rest...> {

	using  base = expanse<Rest...>;

	inline void checkAll() {
		checker<expanse<First, Rest...>, First>::impl(*this);
		std::cout << " ";
		base::checkAll();
	}
};

template <class First>
struct expanse<First> {

	inline void checkAll() { 
		checker<expanse<First>, First>::impl(*this);
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