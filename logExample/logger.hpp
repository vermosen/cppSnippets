// logger.h

#ifndef LOGGER_H
#define LOGGER_H

#include <boost/log/common.hpp>

enum severity_level
{
	sev_debug,
	sev_info,
	sev_warning,
	sev_error,
	sev_fatal
};

// this macro returns the logger stream
// example: LOG(debug) << "stuff to log";
#define LOG(lvl) BOOST_LOG_SEV(Logger::getLogger(), lvl)

class Logger
{
public:
	typedef boost::log::sources::severity_logger<severity_level> MyLogger;

	static void initialize();
	static MyLogger& getLogger() { return _logger; }

private:
	static bool _initialized;
	static MyLogger _logger;
};



#endif // LOGGER_H