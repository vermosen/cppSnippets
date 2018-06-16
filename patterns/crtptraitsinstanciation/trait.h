#pragma once

#include "trait_fwd.h"

template <typename> struct derived;

template <>
struct value<derived<double>> {
	typedef typename double type;
};
