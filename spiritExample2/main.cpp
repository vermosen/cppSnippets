#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/chrono.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix/bind.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <map>

#define BOOST_SPIRIT_DEBUG

namespace dtcc
{
	const int ENUM_NOT_FOUND = -1;  const std::string NEW = "  ";

	template <typename _enum>
	class enumConversions;

	template <typename _enum>
	struct EnumManager
	{
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
	std::map<_enum, std::string> EnumManager<_enum>::initializeEnumToStringMap()
	{
		std::map<_enum, std::string> m;
		for (const auto& x : enumConversions<_enum>::enumToStringVector)
			m[x.first] = x.second;
		return m;
	}

	template <typename _enum>
	const std::map<std::string, _enum> EnumManager<_enum>::stringToEnumMap = EnumManager<_enum>::initializeStringToEnumMap();

	template<typename _enum>
	std::map<std::string, _enum> EnumManager<_enum>::initializeStringToEnumMap()
	{
		std::map<std::string, _enum> m;
		for (const auto& x : enumConversions<_enum>::enumToStringVector)
			m[x.second] = x.first;
		return m;
	}

	template <typename _enum>
	std::string EnumManager<_enum>::toString(const _enum en)
	{
		auto it = EnumManager<_enum>::enumToStringMap.find(en);  // std::map::find is the fastest lookup method
		if (it != EnumManager<_enum>::enumToStringMap.end())
			return it->second;
		return "[[[_enum to string not found." + NEW + "Programmer made an error]]]";
	}

	template<typename _enum>
	_enum EnumManager<_enum>::toEnum(const std::string& str)
	{
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
}

template<typename _enum, class = typename std::enable_if<std::is_enum<_enum>::value>::type>
std::ostream & operator << (std::ostream & os, const _enum en) {
	return os << dtcc::enumConversions<_enum>::toString(en);
}

namespace dtcc
{
	namespace database
	{
		enum class action : char
		{
			  _new		= 1
			, cancel	= 2
			, correct	= 3
		};
	}

	template <>
	const std::vector<std::pair<database::action, std::string> >
		enumConversions<database::action>::enumToStringVector =
	{
		 { database::action::_new	, "NEW" }
		,{ database::action::cancel	, "CANCEL" }
		,{ database::action::correct, "CORRECT" }
	};
}

namespace test
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	///////////////////////////////////////////////////////////////////////////
	//  Our employee struct
	///////////////////////////////////////////////////////////////////////////
	//[tutorial_employee_struct
	struct record
	{
		int64_t DISSEMINATION_ID;
		boost::optional<int64_t> ORIGINAL_DISSEMINATION_ID;
		dtcc::database::action ACTION;
		std::string surname;
		std::string optionalStr;
	};
	//]
}

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen. This has to
// be in global scope.

//[tutorial_employee_adapt_struct
BOOST_FUSION_ADAPT_STRUCT(
	test::record,
	(int64_t, DISSEMINATION_ID)
	(boost::optional<int64_t>, ORIGINAL_DISSEMINATION_ID)
	(dtcc::database::action, ACTION)
	(std::string, surname)
	(std::string, optionalStr)
)
//]

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::action, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::action a) { return std::string(); }

	static void post(dtcc::database::action& d, std::string const& v)
	{
		d = dtcc::EnumManager<dtcc::database::action>::toEnum(v);
	}

	static void fail(dtcc::database::action&) {}
};

namespace test
{
	///////////////////////////////////////////////////////////////////////////////
	//  Our employee parser
	///////////////////////////////////////////////////////////////////////////////
	//[tutorial_employee_parser
	template <typename Iterator>
	struct recordGrammar : qi::grammar<Iterator, record(), ascii::space_type>
	{
		recordGrammar() : recordGrammar::base_type(start)
		{
			quoted_string			%= qi::lexeme['"' >> +(ascii::char_ - '"')	>> '"'	];
			quoted_opt_string		%= qi::lexeme['"' >> *(ascii::char_ - '"')	>> '"'	];
			quoted_int64			%= qi::lexeme['"' >> qi::int_				>> '"'	];
			quoted_optional_int64	%= qi::lexeme['"' >> -(qi::int_)			>> '"'	];
			quoted_action			%= qi::lexeme['"' >> +(ascii::char_ - '"')	>> '"'	];

			start %=
				quoted_int64			>> "," >>
				quoted_optional_int64	>> "," >>
				quoted_action			>> "," >>
				quoted_string			>> "," >>
				quoted_opt_string;
		}

		qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
		qi::rule<Iterator, std::string(), ascii::space_type> quoted_opt_string;
		qi::rule<Iterator, int64_t()> quoted_int64;
		qi::rule<Iterator, int64_t()> quoted_optional_int64;
		qi::rule<Iterator, std::string(), ascii::space_type> quoted_action;
		qi::rule<Iterator, record(), ascii::space_type> start;
	};
	//]
}

static const std::string rec1("\"58919739\",\"58919739\",\"NEW\",\"vermosen\",\"\"");
static const std::string rec2("\"58919739\",\"\",\"CANCEL\",\"vermosen\",\"\"");
boost::chrono::high_resolution_clock::time_point start;

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////

int main()
{
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "\t\tA tradeRecord parser for Spirit...\n\n";
	std::cout << "/////////////////////////////////////////////////////////\n\n";

	typedef std::string::const_iterator iterator_type;

	test::recordGrammar<iterator_type> g; // Our grammar

	start = boost::chrono::high_resolution_clock::now();
	int i = 0;  while (i++ < 1)
	{
		test::record emp1;
		std::string::const_iterator iter1 = rec1.begin();
		std::string::const_iterator end1 = rec1.end();

		if (phrase_parse(iter1, end1, g, boost::spirit::ascii::space, emp1) && iter1 == end1)
		{
			std::cout << "DISSEMINATION_ID: " << emp1.DISSEMINATION_ID << std::endl;
			std::cout << "ORIGINAL_DISSEMINATION_ID: " << emp1.ORIGINAL_DISSEMINATION_ID << std::endl;
			std::cout << "ACTION: " << emp1.ACTION << std::endl;
			std::cout << "employee surname: " << emp1.surname << std::endl;
			std::cout << "employee opt: " << emp1.optionalStr << std::endl;
		}
		else
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "-------------------------\n";
		}

		test::record emp2;
		std::string::const_iterator iter2 = rec2.begin();
		std::string::const_iterator end2 = rec2.end();

		if (phrase_parse(iter2, end2, g, boost::spirit::ascii::space, emp2) && iter2 == end2)
		{
			std::cout << "DISSEMINATION_ID: " << emp2.DISSEMINATION_ID << std::endl;
			std::cout << "ORIGINAL_DISSEMINATION_ID: " << emp2.ORIGINAL_DISSEMINATION_ID << std::endl;
			std::cout << "ACTION: " << emp2.ACTION << std::endl;
			std::cout << "employee surname: " << emp2.surname << std::endl;
			std::cout << "employee opt: " << emp2.optionalStr << std::endl;
		}
		else
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "-------------------------\n";
		}
	}

	std::cout << "Conversion done in " << boost::chrono::duration_cast<boost::chrono::milliseconds> (
		boost::chrono::high_resolution_clock::now() - start) << std::endl;

	system("pause");
	return 0;
}
