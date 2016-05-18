/*
 *
 * signalExample
 * Copyright (C) 2015 Jean-Mathieu Vermosen
 *
 */

#ifndef observable_hpp
#define observable_hpp

#include <boost/signals2.hpp>
#include <boost/thread.hpp>

// template without any return type
template <typename T>
class observable : public boost::signals2::signal< void(T) > {

public:

	virtual ~observable() {};

	void operator ()(T i) {					// overload operator ()

		#ifdef DEBUG
		std::cout << "sending signal from thread " << boost::this_thread::get_id() << std::endl;
		#endif

		this->boost::signals2::signal<void(T)>::operator()(i);

	}

	void registerWith(observer<T> & rc) {
	
		this->connect(boost::signals2::signal<void(T)>::slot_type(				// handshake
			&observer<T>::slot, &rc, _1));
	
	}
};

#endif
