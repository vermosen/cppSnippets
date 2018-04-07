
#include <vector>
#include <map>
#include <iterator>

#include <iterator>

template<typename double, typename Iterator>
struct foo

typename std::iterator_traits<Iterator>::value_type
sum(Iterator begin, Iterator end)
{
	using value_type = typename std::iterator_traits<Iterator>::value_type;
	value_type s = value_type();
	for (Iterator it = begin; it != end; it++) {
		s += *it;
	}
	return s;
}

template<typename T, template <typename, typename...> class It = std::vector<T>::const_iterator>
struct foo 
{
	It<T, std::vector<T>>* m_it;
};

//template<template <typename, typename...> class Container>
//struct foo<double, Container>
//{
//	typename Container<double>::const_iterator* m_it;
//};


int main() {

	foo<double> t;
	foo<std::vector<double>> t2;
	return 0;
}