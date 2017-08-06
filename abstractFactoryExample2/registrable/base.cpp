#include "base.hpp"

#include "../abstractFactory.hpp"

#pragma init_seg(compiler)
template<>
boost::shared_ptr<factory < base, std::tuple<int, std::string> >::map_type>
	factory < base, std::tuple<int, std::string> >::map_ = 
	boost::shared_ptr<factory < base, std::tuple<int, std::string> >::map_type>(new factory::map_type());
