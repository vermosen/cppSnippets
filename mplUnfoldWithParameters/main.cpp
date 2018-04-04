#include <iostream>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>

struct item1 {};
struct item2 {};
struct item3 {};
struct item4 {};

template<bool T>
struct client {
	void haha() { std::cout << "I am a client" << std::endl; }
};

template <bool Pdu, template <bool> class T, typename ... Args>
struct mapper;

template <bool Pdu, template <bool> class T, typename First, typename ... Rest>
struct mapper<Pdu, T, First, Rest...> : public mapper<Pdu, T, Rest ...> {
	using mapper<Pdu, T, Rest...>::foo;
	inline mapper() : mapper<Pdu, T, Rest...>() {}
	virtual ~mapper() {}
	void foo(First f) { std::cout << "HW" << std::endl; }
};

template <bool Pdu, template <bool> class T>
struct mapper<Pdu, T> : public T<Pdu> {
	inline mapper() {}
	virtual ~mapper() {}
	void foo() { static_assert(false, "not for use"); }
};

template < class T1, class T2>
struct folder;

template < template <bool> class Cli, typename... Feeds, typename Feed >
struct folder<mapper<true, Cli, Feeds... >, Feed> {
	typedef mapper<true, Cli, Feeds..., Feed> type;
};

typedef boost::mpl::vector<item1, item2, item3, item4> seq;

struct builder {
public:
	// this one builds the pdu base class
	typedef typename boost::mpl::fold<
		seq,
		mapper<true, client>,
		folder<boost::mpl::_1, boost::mpl::_2>
	>::type type;
};

int main() {

	typename builder::type m;

	m.foo(item1());
	m.foo(item2());
	m.foo(item3());
	m.foo(item4());
	m.haha();

	return 0;
}