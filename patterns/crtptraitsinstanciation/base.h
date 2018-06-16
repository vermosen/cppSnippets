#pragma once

#include "trait_fwd.h"

template<typename child>
struct crtp {
	void foo(typename value<typename child::value_type>::type t) {
		static_cast<child&>(*this).fooImpl(t);
	}
};
