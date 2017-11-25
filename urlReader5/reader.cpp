#include "reader.hpp"

const boost::regex reader::expr_("(Content-Length: )(\\d+$)(\r)");