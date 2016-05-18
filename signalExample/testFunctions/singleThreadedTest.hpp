/*
 *
 * signalExample
 * Copyright (C) 2015 Jean-Mathieu Vermosen
 *
 */

#ifndef single_threaded_test_hpp
#define single_threaded_test_hpp

#include "signals/observer.hpp"
#include "signals/observable.hpp"

void singleThreadedTest() {

	observable<int> send;
	observer<int> rec;
	
	send.connect(observable<int>::slot_type(		// handshake
		&observer<int>::slot, &rec, _1));

	send(0);														// calls the receiver

}

#endif