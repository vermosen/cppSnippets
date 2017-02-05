#ifndef TRADE_RECORD_HPP_
#define TRADE_RECORD_HPP_

#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "enums/action.hpp"
#include "enums/cleared.hpp"
#include "enums/collateralization.hpp"

namespace dtcc
{
	namespace database
	{
		typedef boost::posix_time::ptime tPTime;
		typedef boost::tuple<int, int, int, int, int, int> pTimeAdaptator;

		typedef boost::optional<boost::gregorian::date> tDate;
		typedef boost::optional<boost::tuple<int, int, int> > dateAdaptator;

		struct record
		{
			int DISSEMINATION_ID;
			boost::optional<int> ORIGINAL_DISSEMINATION_ID;
			action ACTION;
			tPTime EXECUTION_TIMESTAMP;
			cleared CLEARED;
			collateralization INDICATION_OF_COLLATERALIZATION;
			boost::optional<bool> INDICATION_OF_END_USER_EXCEPTION;
			bool INDICATION_OF_OTHER_PRICE_AFFECTING_TERM;
			bool BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS;
			tDate EFFECTIVE_DATE;
			tDate END_DATE;
		};
	}
}

#endif
