#pragma once

#include "factory.h"

namespace test {
	enum class foo {
		one = 1
		, five = 5
		, three = 3
	};
}

DECLARE_ENUM_ALL(test::foo, std::int8_t, char const*,
(one, "one")
(five, "five")
(three, "three")
)

