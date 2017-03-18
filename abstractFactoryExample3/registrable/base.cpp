#include "registrable/base.hpp"

#include "abstractFactory.hpp"

#pragma init_seg(compiler)
template<>
factory<base, std::string, std::tuple<int, int> >::map_type *
	factory<base, std::string, std::tuple<int, int> >::map_ = new factory::map_type();
