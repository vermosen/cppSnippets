/*
 *
 * signalExample
 * Copyright (C) 2015 Jean-Mathieu Vermosen
 *
 */

#ifndef observer_hpp
#define observer_hpp

#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

// template version
template<typename T>
class observer {

public:

	virtual void slot(T i) {};
	virtual ~observer() {};

};
#endif
