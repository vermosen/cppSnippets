#ifdef _WIN32
#include "StdAfx.h"
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <string>
#include <boost/optional/optional_io.hpp>

#include "tradeRecordGrammar.hpp"

static const std::string rec("\"123456\",\"\",\"CANCEL\",\"2017-01-24T05:47:46\",\"U\",\"OC\",\"\",\"Y\",\"N\",\"2016-10-12\",\"\"");

int main()
{
	typedef std::string::const_iterator iterator_type;
	typedef record_parser<iterator_type> record_parser;

	record_parser g;
	dtcc::database::record result;
	iterator_type iter = rec.begin();
	iterator_type end = rec.end();

	if (phrase_parse(iter, end, g, ascii::space, result) && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "DISSEMINATION_ID: " << result.DISSEMINATION_ID << std::endl;

		if (!result.ORIGINAL_DISSEMINATION_ID)
		{
			std::cout << "ORIGINAL_DISSEMINATION_ID: " << "(none)" << std::endl;
		}
		else
		{
			std::cout << "ORIGINAL_DISSEMINATION_ID: " << result.ORIGINAL_DISSEMINATION_ID << std::endl;
		}

		std::cout << "ACTION: " << result.ACTION << std::endl;
		std::cout << "EXECUTION_TIMESTAMP: " << result.EXECUTION_TIMESTAMP << std::endl;
		std::cout << "CLEARED: " << result.CLEARED << std::endl;

		std::cout << "INDICATION_OF_COLLATERALIZATION: " << result.INDICATION_OF_COLLATERALIZATION << std::endl;
		if (!result.INDICATION_OF_END_USER_EXCEPTION)
			std::cout << "INDICATION_OF_END_USER_EXCEPTION: (none)" << std::endl;
		else
			std::cout << "INDICATION_OF_END_USER_EXCEPTION: " << result.INDICATION_OF_END_USER_EXCEPTION.get() << std::endl;

		std::cout << "INDICATION_OF_OTHER_PRICE_AFFECTING_TERM: " << result.INDICATION_OF_OTHER_PRICE_AFFECTING_TERM << std::endl;
		std::cout << "BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS: " << result.BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS << std::endl;

		//if (!result.EXECUTION_VENUE)
		//	std::cout << "EXECUTION_VENUE: (none)" << std::endl;
		//else
		//	std::cout << "EXECUTION_VENUE: " << result.EXECUTION_VENUE.get() << std::endl;

		if (!result.EFFECTIVE_DATE)
		{
			std::cout << "EFFECTIVE_DATE: " << "(none)" << std::endl;
		}
		else
		{
			std::cout << "EFFECTIVE_DATE: " << result.ORIGINAL_DISSEMINATION_ID << std::endl;
		}

		if (!result.END_DATE)
		{
			std::cout << "END_DATE: " << "(none)" << std::endl;
		}
		else
		{
			std::cout << "END_DATE: " << result.ORIGINAL_DISSEMINATION_ID << std::endl;
		}

		std::cout << "\n-------------------------\n";
	}
	else
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing failed\n";
		std::cout << "-------------------------\n";
	}

	system("pause");
	return 0;
}
