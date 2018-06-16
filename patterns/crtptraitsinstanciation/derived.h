#pragma once

#include <iostream>

#include "derived_fwd.h"
#include "base.h"
#include "trait.h"

template <>
struct derived<double> : public crtp<derived<double>> {

	typedef double value_type;

	void fooImpl(typename value<double>::type t) {
		std::cout << t << std::endl;
	}
};
