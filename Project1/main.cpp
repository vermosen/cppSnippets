#include <string>
#include <iostream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace bmi = boost::multi_index;

struct employee {

	employee(int id_, std::string name_, int age_) 
		: id(id_), name(name_), age(age_) {}

	int         id;
	std::string name;
	int         age;
};

/* tag declaration */
namespace tags {
	struct id {};
	struct name {};
	struct age {};
}

typedef bmi::multi_index_container<				// forget about the ugly declaration...
	employee,
	bmi::indexed_by<
		bmi::ordered_unique<
			bmi::tag<tags::id>, 
			BOOST_MULTI_INDEX_MEMBER(employee, int, id)
		>,
		bmi::ordered_non_unique<
			bmi::tag<tags::name>, 
			BOOST_MULTI_INDEX_MEMBER(employee, std::string, name)
		>,
		bmi::ordered_non_unique<
			bmi::tag<tags::age>, 
			BOOST_MULTI_INDEX_MEMBER(employee, int, age)
		> 
	>
> employee_set;

int main()
{
	employee_set es; 

	es.insert(employee(0, "Joe", 31));
	es.insert(employee(1, "Robert", 27));
	es.insert(employee(2, "John", 40));

	std::cout 
		<< "Joe is " 
		<< es.get<tags::name>().find("Joe")->age 
		<< " years old..."
		<< std::endl;

	std::cout
		<< "The oldest person is: "
		<< es.get<tags::age>().rbegin()->name		// reverse begin iterator returns the largest element wrt the key
		<< std::endl;

	return 0;
}