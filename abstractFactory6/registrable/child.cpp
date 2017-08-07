#include "registrable/child.hpp"

registerType<base, int, child, std::string>
    child::register1_(1);

//registerType<base, int, child> 
//    child::register2_;