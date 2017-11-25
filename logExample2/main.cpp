#include <iostream>
#include <string>

#include "enum_utils.hpp"
#include "logger.hpp"

int main()
{
	foo::logger::initialize("test_%Y%m%d.log", foo::severity_level::sev_info);
	LOG(foo::sev_error) << "this is a test";
	return 0;
}

