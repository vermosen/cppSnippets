#include "registrable/base.hpp"

#include "abstractFactory.hpp"

#pragma init_seg(compiler)
template<>
factory < base, std::tuple<int, std::string> >::map_type *
	factory < base, std::tuple<int, std::string> >::map_ = new factory::map_type();
