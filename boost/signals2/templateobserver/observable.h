#pragma once

#pragma warning(disable:4996)

#include <boost/signals2.hpp>

#include "observer.h"

template <typename>
class observer;

template<typename T>
class observable {
public:
	using handler = boost::signals2::signal<void(T)>;

	void subscribe(observer<T>& o);
	void notify(T o);
	handler sig;
};

template <typename T>
inline void observable<T>::subscribe(observer<T>& o) {
	sig.connect(std::bind(&observer<T>::update, &o, std::placeholders::_1));
}

template <typename T>
inline void observable<T>::notify(T t) { sig(t); }