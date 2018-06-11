#pragma once

#include <iostream>

#include "base.h"
#include "trait.h"

struct derived : public crtp<derived> {

	void fooImpl(typename value<derived>::type t) {
		std::cout << t << std::endl;
	}
};
