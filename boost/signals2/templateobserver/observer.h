#pragma once

#include <iostream>

template<typename Signature>
class observable;

template <typename T>
class observer {
public:
	observer();

	void bind(observable<T>& o);

	void update(T d);
	T m_value;
};

template <typename T>
inline observer<T>::observer() {}

template <typename T>
inline void observer<T>::bind(observable<T>& o) {
	o.subscribe(*this);
}

template <typename T>
inline void observer<T>::update(T d) {
	m_value = d;
	std::cout
		<< "get value "
		<< m_value
		<< std::endl;
}