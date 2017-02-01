#include "logger.hpp"

#include <iostream>

#include "boost/log/common.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/utility/setup/console.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"
#include "boost/log/attributes/timer.hpp"
#include "boost/log/attributes/named_scope.hpp"
#include "boost/log/sources/logger.hpp"
#include "boost/log/support/date_time.hpp"


bool Logger::_initialized = false;
Logger::MyLogger Logger::_logger = Logger::MyLogger();

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
	static const char* const str[] =
	{
		"debug",
		"info",
		"warning",
		"error",
		"fatal"
	};
	if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast< int >(lvl);
	return strm;
}