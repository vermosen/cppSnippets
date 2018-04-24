#include <string>
#include <iostream>
#include <map>
#include <chrono>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>

typedef std::chrono::microseconds duration_type;
typedef std::chrono::system_clock clock_type;
typedef std::chrono::time_point<clock_type> point_type;

inline void elapsed(const char * test, point_type p) {
	auto t = std::chrono::duration_cast<duration_type>(clock_type::now() - p);
	std::cout
		<< test
		<< " took: "
		<< t.count()
		<< " microseconds"
		<< std::endl;
}

namespace bmi = boost::multi_index;

struct instrument {
	instrument(int lid_, int uid_, int pid_)
		: lid(lid_), uid(uid_), pid(pid_) {}

	int         lid;		// local id
	int         uid;		// underlying id
	int         pid;		// product id
};

/* tag declaration */
namespace tags {
	struct composite {};
	struct lid {};
	struct uid {};
	struct pid {};
}

typedef bmi::multi_index_container<				// forget about the ugly declaration...
	instrument,
	bmi::indexed_by<
		bmi::ordered_unique<
			bmi::tag<tags::composite>,
			bmi::composite_key<instrument,
				BOOST_MULTI_INDEX_MEMBER(instrument, int, pid),
				BOOST_MULTI_INDEX_MEMBER(instrument, int, uid)
			>
		>
	> 
> 
boost_set;

// the standard equivalent
typedef typename std::map<int, std::map<int, int> > standard_set;

int main()
{
	boost_set bs;
	standard_set ss;
	int k = 0; int l = 0; 

	// step 1 - insert

	point_type start = clock_type::now();
	// insertion stl
	for (int i = 0; i < 5000; i++) {
		auto it = ss.insert(std::make_pair(i, std::map<int, int>()));
		for (int j = 0; j < 5000; j++) {
			it.first->second.insert(std::make_pair(j, l++));
		}
	}
	elapsed("stl insertion", start);

	// insertion boost
	k = 0; l = 0;
	start = clock_type::now();
	for (int i = 0; i < 5000; i++) {
		for (int j = 0; j < 5000; j++) {
			bs.insert(instrument(l++, i, k++));
		}
	}
	elapsed("boost insertion", start);

	// step 2 - lookup
	int ttt = 0;
	start = clock_type::now();
	for (int i = 0; i < 5000; i++) {
		ttt += ss[i][i];
	}
	
	elapsed("stl lookup ", start);
	std::cout << ttt << std::endl;

	ttt = 0;
	start = clock_type::now();
	auto& index = bs.get<tags::composite>();
	for (int i = 0; i < 5000; i++) {
		boost_set::iterator it = index.find(std::make_tuple(i, i));
		ttt += it->lid;
	}

	elapsed("boost lookup ", start);
	std::cout << ttt << std::endl;
	system("pause");
	return 0;
}