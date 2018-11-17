#pragma once

#pragma warning(disable:4996)

#include <boost/signals2.hpp>

#include "observer.h"

class observer;

class observable {
public:
	using handler = boost::signals2::signal<void(double)>;

	void subscribe(observer& o);
	void notify(double o);

	handler sig;
};
