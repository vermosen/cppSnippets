#include <iostream>
#include <boost/foreach.hpp> 
#include <boost/preprocessor.hpp>

#define X_DEFINE_ENUM_TOSTRING_CASE(r, data, elem)							\
    case elem : return BOOST_PP_STRINGIZE(elem);

#define DEFINE_TEMPLATE_ENUM(r, type, elem)									\
    template<> const char name<type::elem>[] = BOOST_PP_STRINGIZE(elem);

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(type, enumerators)				\
enum type : uint32_t														\
{																			\
    BOOST_PP_SEQ_ENUM(enumerators)											\
};																			\
inline const char * toChar(type v)											\
{																			\
	switch (v)																\
	{																		\
	BOOST_PP_SEQ_FOR_EACH(													\
		X_DEFINE_ENUM_TOSTRING_CASE,										\
		type,																\
		enumerators															\
	)																		\
	default: return "[Unknown " BOOST_PP_STRINGIZE(type) "]";				\
	}																		\
}																			\
template<type> const char name[] = "Invalid value"; 						\
BOOST_PP_SEQ_FOR_EACH(														\
	DEFINE_TEMPLATE_ENUM,													\
	type,																	\
	enumerators)															\
inline std::ostream & operator << (std::ostream & s, type n)				\
{																			\
	s << toChar(n);															\
    return s;																\
}

DEFINE_ENUM_WITH_STRING_CONVERSIONS(myEnum, (AAA)(BBB)(CCC)(DDD))

int main()
{
	// Prints "DDD"
	std::cout << name<myEnum::DDD> << '\n';

	// Prints "Invalid MyEnum value"
	std::cout << name<static_cast<myEnum>(0x12345678)> << '\n';

	// Prints "BBB"
	std::cout << myEnum::BBB << std::endl;

	// do not compile
	//std::cout << myEnum::EEE << std::endl;

	return 0;
}