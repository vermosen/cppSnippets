#include <iostream>
#include <string>

#include "enum_utils.hpp"
#include "logger.hpp"

enum class TestEnum
{
	ValA,
	ValB
};

template<> sj::enum_strings<TestEnum>::maptype
sj::enum_strings<TestEnum>::data =
{
	{ TestEnum::ValA, "ValA" },
	{ TestEnum::ValB, "ValB" }
};

namespace myns
{

	enum class TestEnumNS
	{
		ValC,
		ValD
	};
	using ::operator<<;
}

template<> sj::enum_strings<myns::TestEnumNS>::maptype
sj::enum_strings<myns::TestEnumNS>::data =
{
	{ myns::TestEnumNS::ValC, "ValC" },
	{ myns::TestEnumNS::ValD, "ValD" }
};


int main()
{
	LOG(sev_debug) << TestEnum::ValB;
	std::cout << TestEnum::ValB << std::endl;

	// this line doesn't compile for some reason!
	LOG(sev_debug) << myns::TestEnumNS::ValC;
	// std::cout works without problems
	std::cout << myns::TestEnumNS::ValC << std::endl;

	return 0;
}