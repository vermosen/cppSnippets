#pragma once
#ifndef ENUM_MANAGER_HPP
#define ENUM_MANAGER_HPP

#include "foo.h"

int main() {
	std::cout << utils::factory<test::foo>::key(test::foo::one) << std::endl;
	auto t2 = utils::factory<test::foo>::value("five");
	return 0;
}

#endif /* ENUMMANAGER_HPP */