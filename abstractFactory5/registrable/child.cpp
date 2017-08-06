#include "registrable/child.hpp"

registerType<base, std::string, child, std::string, int>
    child::register_(std::string("A"));
