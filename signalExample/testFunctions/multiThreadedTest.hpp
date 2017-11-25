/*
*
* signalExample
* Copyright (C) 2015 Jean-Mathieu Vermosen
*
*/

#ifndef multi_threaded_test_hpp
#define multi_threaded_test_hpp

#include <signals/observer.hpp>
#include <signals/observable.hpp>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

// a sender
void sendingLoopInt(int seed, observable<int> & send) {

	for (int i = seed;; i++) send(i);

};

void multiThreadedTest() {

	observable<int> send;
	observer<int> rec;

	std::cout << "this thread id: " << boost::this_thread::get_id();

	send.connect(boost::signals2::signal<void(int)>::slot_type(			// handshake
		&observer<int>::slot, &rec, _1));
	
	boost::thread sendingThread1(sendingLoopInt, 1,   boost::ref(send));	// create the sender thread
	boost::thread sendingThread2(sendingLoopInt, 100, boost::ref(send));

	sendingThread1.join();												// detach both threads
	sendingThread2.join();

}

#endif
