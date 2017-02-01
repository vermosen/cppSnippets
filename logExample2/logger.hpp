// logger.h

#ifndef LOGGER_H
#define LOGGER_H

#include <boost/log/common.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/phoenix/bind/bind_function_object.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

namespace foo
{
	enum severity_level
	{
		sev_debug,
		sev_info,
		sev_warning,
		sev_error,
		sev_fatal
	};
}

namespace
{
	// define the keywords to use in filter and formatter
	BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", foo::severity_level)
	BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::log::attributes::utc_clock::value_type)
	BOOST_LOG_ATTRIBUTE_KEYWORD(processid, "PID", boost::log::attributes::current_process_id::value_type)
	BOOST_LOG_ATTRIBUTE_KEYWORD(threadid, "Thread", boost::log::attributes::current_thread_id::value_type)
}

// this macro returns the logger stream
// example: LOG(debug) << "stuff to log";
#define LOG(lvl) BOOST_LOG_SEV(foo::logger::getLogger(), lvl)

namespace foo
{
	class logger
	{
	public:
		typedef boost::log::sources::severity_logger<severity_level> logger_t;
		typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend> sink_t;

		static void initialize(const std::string & fileName, severity_level threshold);
		static logger_t& getLogger();
		// the filter used to filter log records according to level
		static bool levelFilter(boost::log::value_ref< severity_level, tag::severity> const& level);

	private:
		static bool initialized_;
		static logger_t logger_;
		static severity_level threshold_;
	};
}

#endif // LOGGER_H