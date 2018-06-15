
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <string>
#include <iostream>
#include <typeinfo>

using namespace boost::fusion;


template<class Pred, class Fun>
struct filter {
	Pred pred_;
	Fun& fun_;
	const char* key_;
	const char* val_;

	filter(Pred p, Fun& f, const char* key, const char* val)
		: pred_(p)
		, fun_(f)
		, key_(key)
		, val_(val)
	{}

	template<class Pair>
	void operator()(Pair& pair) const {
		if (pred_(pair.second, key_))
			fun_(pair, val_);
	}
};

template<class Pred, class Fun>
filter<Pred, Fun> find_if(Pred p, Fun& f, const char* key, const char* val) {
	return filter<Pred, Fun>(p, f, key, val);
}

typedef map<
	  pair<int, const char*>
	, pair<double, const char*>
	, pair<std::string, const char*>
> map_type;

template<typename T>
struct callback;

template<>
struct callback<double> {
	static void invoke(const char* fld, const char* val) {
		// call the correct bbg function
	}
};

template<>
struct callback<int> {
	static void invoke(const char* fld, const char* val) {
		// call the correct bbg function
	}
};

template<>
struct callback<std::string> {
	static void invoke(const char* fld, const char* val) {
		// call the correct bbg function
	}
};


struct fun {
	template<class First, class Second>
	void operator()(pair<First, Second>& t, const char* value) const {
		callback<First>::invoke(t.second, value);
	}
};

struct mypred {
	template<class T>
	bool operator()(T const& t, const char* value) const {
		return t == value;
	}
};

int main(int argc, char** argv) {

	map_type m(
		  make_pair<int>("ABC")
		, make_pair<double>("DEF")
		, make_pair<std::string>("GHI")
	);

	const char* key = "GHI";
	const char* value = "12.35";

	for_each(m, find_if(mypred(), fun(), key, value));

	return 0;
}
