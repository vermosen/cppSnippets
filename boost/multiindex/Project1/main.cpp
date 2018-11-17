
#include <iostream>

#include <boost/noncopyable.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

struct test : public boost::noncopyable {

    int m_one;
    std::string m_two;
};

/* tag declaration */
namespace tags {
	struct one {};
	struct two {};
}

namespace bmi = boost::multi_index;

typedef bmi::multi_index_container<				// forget about the ugly declaration...
    test,
	bmi::indexed_by<
		bmi::hashed_unique<
			bmi::tag<tags::one>,
			BOOST_MULTI_INDEX_MEMBER(test, int, m_one)
		>,
		bmi::hashed_unique<
			bmi::tag<tags::two>,
			BOOST_MULTI_INDEX_MEMBER(test, std::string, m_two)
		>
	>
> test_set;

int main() {

    test_set set;
    test three; three.m_one = 3; three.m_two = "three";
    //set.emplace(1, "one");
    //set.emplace_back(2, "two");
    set.insert(std::move(three));

    return 0;
}
