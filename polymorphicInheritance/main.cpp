#include <boost/mpl/vector.hpp>

struct item1 {};
struct item2 {};
struct item3 {};
struct item4 {};
struct item5 {};

namespace tags {
	struct type1 {};
	struct type2 {};
	struct type3 {};
}

template <typename T>
struct cathegory;

template <>
struct cathegory<item1> {
	typedef typename tags::type1 type;
};

template <>
struct cathegory<item2> {
	typedef typename tags::type1 type;
};

template <>
struct cathegory<item3> {
	typedef typename tags::type3 type;
};

template <>
struct cathegory<item4> {
	typedef typename tags::type2 type;
};

template <>
struct cathegory<item5> {
	typedef typename tags::type1 type;
};

template <template Cathegory, typename T>
struct mapper;

template <typename T>
struct mapper<tags::type1> {


};

template <typename T>
struct mapper<tags::type2> {


};

template <typename T>
struct mapper<tags::type3> {


};

namespace nodes {}