#include <iostream>
#include <string>
#include <vector>
#include <map>

const int ENUM_NOT_FOUND = -1;  const std::string NEW = "  ";

template <typename Enum>
class EnumConversions;

template <typename Enum>
struct EnumManager {
	static std::string toString(const Enum en);
	static Enum toEnum(const std::string& str);
	static const std::map<Enum, std::string> enumToStringMap;
	static const std::map<std::string, Enum> stringToEnumMap;
private:
	static std::map<Enum, std::string> initializeEnumToStringMap();
	static std::map<std::string, Enum> initializeStringToEnumMap();
};

template <typename Enum>
const std::map<Enum, std::string> EnumManager<Enum>::enumToStringMap = EnumManager<Enum>::initializeEnumToStringMap();

template<typename Enum>
std::map<Enum, std::string> EnumManager<Enum>::initializeEnumToStringMap() {
	std::map<Enum, std::string> m;
	for (const auto& x : EnumConversions<Enum>::enumToStringVector)
		m[x.first] = x.second;
	return m;
}

template <typename Enum>
const std::map<std::string, Enum> EnumManager<Enum>::stringToEnumMap = EnumManager<Enum>::initializeStringToEnumMap();

template<typename Enum>
std::map<std::string, Enum> EnumManager<Enum>::initializeStringToEnumMap() {
	std::map<std::string, Enum> m;
	for (const auto& x : EnumConversions<Enum>::enumToStringVector)
		m[x.second] = x.first;
	return m;
}

template <typename Enum>
std::string EnumManager<Enum>::toString(const Enum en) {
	auto it = EnumManager<Enum>::enumToStringMap.find(en);  // std::map::find is the fastest lookup method
	if (it != EnumManager<Enum>::enumToStringMap.end())
		return it->second;
	return "[[[Enum to string not found." + NEW + "Programmer made an error]]]";
}

template<typename Enum>
Enum EnumManager<Enum>::toEnum(const std::string& str) {
	auto it = EnumManager<Enum>::stringToEnumMap.find(str);
	if (it != EnumManager<Enum>::stringToEnumMap.end())
		return it->second;
	return static_cast<Enum> (ENUM_NOT_FOUND);
}

template <typename Enum>
class EnumConversions : public EnumManager<Enum> {
private:
	EnumConversions();  // to prevent instantiation
public:
	static const std::vector<std::pair<Enum, std::string>> enumToStringVector;
};

template<typename Enum, class = typename std::enable_if<std::is_enum<Enum>::value>::type>
std::ostream& operator << (std::ostream& os, Enum en) {
	return os << EnumConversions<Enum>::toString(en);
}

template<typename Enum, class = typename std::enable_if<std::is_enum<Enum>::value>::type>
std::istream& operator >> (std::istream& is, Enum& en) {
	std::string buf;
	is >> buf;
	en = EnumConversions<Enum>::toEnum(buf);
	return is;
}

enum class Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

template <>
const std::vector<std::pair<Day, std::string> > EnumConversions<Day>::enumToStringVector = {
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
const std::vector<std::pair<DayFr, std::string> > EnumConversions<DayFr>::enumToStringVector = {
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
	std::cout << static_cast<Day> (Day::Monday) << std::endl;
	std::cout << static_cast<DayFr> (DayFr::Monday) << std::endl;
	system("pause");
}