#include "registrable/child.hpp"

registerType<base, std::tuple<int, std::string>, child>
child::register_(std::tuple<int, std::string>(1, "A"));
