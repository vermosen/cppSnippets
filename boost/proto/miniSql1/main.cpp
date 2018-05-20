#include <iostream>
#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>
namespace proto = boost::proto;

// table consumer
namespace consumer {
	struct first_name {
		friend std::ostream &operator <<(std::ostream &sout, first_name) {
			return sout << "FIRST_NAME";
		}
	};
	struct last_name {
		friend std::ostream &operator <<(std::ostream &sout, last_name) {
			return sout << "LAST_NAME";
		}
	};
	struct address {
		friend std::ostream &operator <<(std::ostream &sout, address) {
			return sout << "ADDRESS";
		}
	};
}

typedef proto::terminal<consumer::first_name>::type first_name_t;
typedef proto::terminal<consumer::last_name>::type last_name_t;
typedef proto::terminal<consumer::address>::type address_t;

struct first_name;
struct last_name;
struct address;

struct Red : proto::and_<
	  proto::plus<Green, Blue>
	, proto::plus<Blue, Green>
	, proto::plus<Red, Red>
	, proto::terminal<RedTag>
> {};