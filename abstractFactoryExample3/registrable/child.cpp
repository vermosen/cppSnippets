#include "child.hpp"

registerType<base, std::string, child, std::tuple<int, int> >
    child::register_(std::string("A"));
