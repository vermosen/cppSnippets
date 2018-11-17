#include "observable.h"
#include "observer.h"

#include <iostream>

observer::observer() {}

void observer::bind(observable& o) {
	o.subscribe(*this);
}

void observer::update(double d) {
	std::cout
		<< "get value "
		<< d
		<< std::endl;
}