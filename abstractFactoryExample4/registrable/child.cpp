#include "registrable/child.hpp"

registerType<base, std::string, child1, std::string, int>
	child1::register_(std::string("A"));