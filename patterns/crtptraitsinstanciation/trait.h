#pragma once

#include "trait_fwd.h"

struct derived;

template <>
struct value<derived> {
	typedef double type;
};
