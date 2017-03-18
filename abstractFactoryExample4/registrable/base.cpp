#include "registrable/base.hpp"

#include "abstractFactory.hpp"

#pragma init_seg(compiler)
template<>
factory<base, std::string, std::string, int>::map_type *
	factory<base, std::string, std::string, int>::map_ = new factory::map_type();
