#include "registrable/child.hpp"

registerType<base, std::string, child, int>
    child::register_("A");
