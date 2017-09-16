#include "registrable/base.hpp"

#include "abstractFactory.hpp"

#pragma init_seg(compiler)
template<>
factory<base, int, std::string>::map_type *
    factory<base, int, std::string>::map_ = new factory::map_type();

template<>
factory<base, int>::map_type *
    factory<base, int>::map_ = new factory::map_type();
