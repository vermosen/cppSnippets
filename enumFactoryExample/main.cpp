#include <iostream>
#include <exception>
#include <boost/foreach.hpp> 
#include <boost/preprocessor.hpp>

#define X_DEFINE_ENUM_TOSTRING_CASE(r, data, elem)							\
    case elem : return BOOST_PP_STRINGIZE(elem);

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)				\
enum name																	\
{																			\
    BOOST_PP_SEQ_ENUM(enumerators)											\
};																			\
																			\
inline const char * toString(name v)										\
{																			\
    switch (v)																\
    {																		\
        BOOST_PP_SEQ_FOR_EACH(												\
            X_DEFINE_ENUM_TOSTRING_CASE,									\
            name,															\
            enumerators														\
        )																	\
        default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";			\
    }																		\
}

DEFINE_ENUM_WITH_STRING_CONVERSIONS(OS_type, (Linux)(Apple)(Windows))

int main()
{
	try
	{
		// approach 1
		OS_type t = Windows;
		std::cout << toString(t) << " " << Apple << std::endl;

		// approach 2

	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}
