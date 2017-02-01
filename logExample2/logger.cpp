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

namespace foo
{
	bool logger::initialized_ = false;
	logger::logger_t logger::logger_ = logger::logger_t();
	severity_level logger::threshold_ = severity_level::sev_info;

	template< typename CharT, typename TraitsT >
	inline std::basic_ostream< CharT, TraitsT >& operator<< (
		std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
	{
		static const char* const str[] =
		{
			"debug",
			"info ",
			"warn ",
			"error",
			"fatal"
		};
		if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
			strm << str[lvl];
		else
			strm << static_cast< int >(lvl);
		return strm;
	}

	logger::logger_t & logger::getLogger() { return logger_; }

	bool logger::levelFilter(boost::log::value_ref<severity_level, tag::severity> const& level)
	{
		return level >= logger::threshold_;
	}

	void logger::initialize(const std::string & fileName, severity_level threshold)
	{
		if (!initialized_)
		{
			// create a logger
			logger_ = logger_t();

			logger::threshold_ = threshold;

			auto c = boost::log::core::get();

			// add attributes
			c->add_global_attribute("TimeStamp", boost::log::attributes::utc_clock());
			c->add_global_attribute("PID", boost::log::attributes::current_process_id());
			c->add_global_attribute("Thread", boost::log::attributes::current_thread_id());

			// add level filter
			c->set_filter(boost::phoenix::bind(&levelFilter, severity.or_none()));

			// create a sink with max size and automatic rotation
			boost::shared_ptr<boost::log::sinks::text_file_backend > backend =
				boost::make_shared<boost::log::sinks::text_file_backend>
				(
					boost::log::keywords::auto_flush = false,
					boost::log::keywords::file_name = fileName,						// filename
					boost::log::keywords::open_mode = std::ios_base::app | std::ios_base::out,
					boost::log::keywords::rotation_size = 10 * 1024 * 1024,			// 10 MB
					boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0)
					);

			// createe the actual sink
			boost::shared_ptr<logger::sink_t> sink(new logger::sink_t(backend));

			// sink formatter
			sink->set_formatter
			(
				boost::log::expressions::stream
				<< "[" << boost::log::expressions::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << " - "
				<< processid << " - " << threadid << "] <" << severity << "> "
				<< boost::log::expressions::smessage
			);

			// add sink
			c->add_sink(sink);
			initialized_ = true;
		}
	}
}
