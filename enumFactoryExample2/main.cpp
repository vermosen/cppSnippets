#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

const int ENUM_NOT_FOUND = -1;  const std::string NEW = "  ";

template <typename _enum>
class enumConversions;

template <typename _enum>
struct EnumManager {
	static std::string toString(const _enum en);
	static _enum toEnum(const std::string& str);
	static const std::map<_enum, std::string> enumToStringMap;
	static const std::map<std::string, _enum> stringToEnumMap;
private:
	static std::map<_enum, std::string> initializeEnumToStringMap();
	static std::map<std::string, _enum> initializeStringToEnumMap();
};

template <typename _enum>
const std::map<_enum, std::string> EnumManager<_enum>::enumToStringMap = EnumManager<_enum>::initializeEnumToStringMap();

template<typename _enum>
std::map<_enum, std::string> EnumManager<_enum>::initializeEnumToStringMap() {
	std::map<_enum, std::string> m;
	for (const auto& x : enumConversions<_enum>::enumToStringVector)
		m[x.first] = x.second;
	return m;
}

template <typename _enum>
const std::map<std::string, _enum> EnumManager<_enum>::stringToEnumMap = EnumManager<_enum>::initializeStringToEnumMap();

template<typename _enum>
std::map<std::string, _enum> EnumManager<_enum>::initializeStringToEnumMap() {
	std::map<std::string, _enum> m;
	for (const auto& x : enumConversions<_enum>::enumToStringVector)
		m[x.second] = x.first;
	return m;
}

template <typename _enum>
std::string EnumManager<_enum>::toString(const _enum en) {
	auto it = EnumManager<_enum>::enumToStringMap.find(en);  // std::map::find is the fastest lookup method
	if (it != EnumManager<_enum>::enumToStringMap.end())
		return it->second;
	return "[[[_enum to string not found." + NEW + "Programmer made an error]]]";
}

template<typename _enum>
_enum EnumManager<_enum>::toEnum(const std::string& str) {
	auto it = EnumManager<_enum>::stringToEnumMap.find(str);
	if (it != EnumManager<_enum>::stringToEnumMap.end())
		return it->second;
	return static_cast<_enum> (ENUM_NOT_FOUND);
}

template <typename _enum>
class enumConversions : public EnumManager<_enum> {
private:
	enumConversions();  // to prevent instantiation
public:
	static const std::vector<std::pair<_enum, std::string>> enumToStringVector;
};

template<typename _enum, class = typename std::enable_if<std::is_enum<_enum>::value>::type>
std::ostream& operator << (std::ostream& os, _enum en) {
	return os << enumConversions<_enum>::toString(en);
}

enum class Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

template <>
const std::vector<std::pair<Day, std::string> > enumConversions<Day>::enumToStringVector = 
{
	  { Day::Monday		, "Monday"		}
	, { Day::Tuesday	, "Tuesday"		}
	, { Day::Wednesday	, "Wednesday"	}
	, { Day::Thursday	, "Thursday"	}
	, { Day::Friday		, "Friday"		}
	, { Day::Saturday	, "Saturday"	}
	, { Day::Sunday		, "Sunday"		}
};

enum class DayFr { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

template <>
const std::vector<std::pair<DayFr, std::string> > enumConversions<DayFr>::enumToStringVector = 
{
	  { DayFr::Monday	, "Lundi"		}
	, { DayFr::Tuesday	, "Mardi"		}
	, { DayFr::Wednesday, "Mercredi"	}
	, { DayFr::Thursday	, "Jeudi"		}
	, { DayFr::Friday	, "Vendredi"	}
	, { DayFr::Saturday	, "Samedi"		}
	, { DayFr::Sunday	, "Dimanche"	}
};

int main() 
{
	std::cout << Day::Monday << std::endl;

	std::stringstream ss;
	ss << Day::Monday;
	std::cout << ss.str() << std::endl;

	auto day = enumConversions<Day>::toEnum("Tuesday");
	std::cout << day << std::endl;

	system("pause");
}