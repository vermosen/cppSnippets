#include <iostream>
#include <type_traits>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/not.hpp>

enum kind {
	  none = 0
	, type_1 = 1
	, type_2 = 2
};

struct message {
	kind m_type;
};

struct item1 {};
struct item2 {};
struct item3 {};
struct item4 {};
struct item5 {};

template <typename T>
struct message_trait;

template <typename T>
struct message_type;

template <> struct message_trait<item1> {
	static constexpr kind value = kind::none;
};

template <> struct message_trait<item2> {
	static constexpr kind value = kind::type_1;
};

template <> struct message_trait<item3> {
	static constexpr kind value = kind::none;
};

template <> struct message_trait<item4> {
	static constexpr kind value = kind::none;
};

template <> struct message_trait<item5> {
	static constexpr kind value = kind::type_2;
};

template<typename T>
struct message_type {
	static constexpr kind value = message_trait<T>::value;
};

// typedef boost::mpl::vector<item1, item2, item3, item4, item5> vec_type;
// optional: transform into the variadic sequence...

struct base {};

template <typename ... Args>
struct mapper;

template <typename First, typename ... Rest>
struct mapper<First, Rest...> : public mapper<Rest ...> {

	using mapper<Rest ...>::my_handle;
	using mapper<Rest ...>::get_message;

	mapper() : mapper<Rest ...>() {}
	virtual ~mapper() {}

	// here, enable if doesn't work
	void my_handle(First v) {
		static_assert(message_type<First>::value == 0, "cannot call the function, stupid!");
	}

	inline void get_message(message m)
	{
		if(message)
		mapper<Rest ...>::get_message(m);
	}
};

template <typename Last>
struct mapper<Last> : public base {

	mapper() : base() {}
	virtual ~mapper() {}

	// here, enable if doesn't work
	void my_handle(Last v) {
		static_assert(message_type<Last>::value == 0, "cannot call the function, stupid!");
	}

	void get_message(message m) { return; }
};

int main() {

	mapper<item1, item2, item3, item4, item5> m;

	m.my_handle(item1());
	m.my_handle(item3());
	m.my_handle(item4());

	m.get_message(message() = { kind::type_2 });

	return 0;
}