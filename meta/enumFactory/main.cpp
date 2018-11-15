#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

// Doubles the parentheses of a Boost.PP sequence,
// i.e. (a, b)(c, d) becomes ((a, b))((c, d))
#define FOO_SEQ_DOUBLE_PARENS_0(...) \
     ((__VA_ARGS__)) FOO_SEQ_DOUBLE_PARENS_1

#define FOO_SEQ_DOUBLE_PARENS_1(...) \
     ((__VA_ARGS__)) FOO_SEQ_DOUBLE_PARENS_0

#define FOO_SEQ_DOUBLE_PARENS_0_END
#define FOO_SEQ_DOUBLE_PARENS_1_END

#define FOO_SEQ_DOUBLE_PARENS(seq) \
    BOOST_PP_CAT(FOO_SEQ_DOUBLE_PARENS_0 seq, _END)

#include <string>
#include <array>
#include <iostream>
#include <utility>
#include <memory>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>

namespace Foo {
namespace Utils {
namespace meta {
namespace enums {
namespace details {

	template <typename T>
	struct size;

	template <typename T, std::size_t S>
	struct create;

	template <typename T>
	struct key_t;

	namespace tags {
		struct key {};
		struct value {};
	}

	template <typename T>
	struct pair {
		typedef typename std::pair<typename key_t<T>::type, T> type;
	};

	template<class T, std::size_t N, std::size_t... Rest>
	struct array_impl {
		static constexpr auto& value = array_impl<T, N - 1, N, Rest...>::value;
	};

	template<class T, std::size_t... Rest>
	struct array_impl<T, 0, Rest...> {
		static constexpr std::array<typename pair<T>::type, sizeof...(Rest)+1> value = {
			create<T, 0>::load(), create<T, Rest>::load()...
		};
	};

	template<class T, std::size_t... Rest>
	constexpr std::array<typename pair<T>::type, sizeof...(Rest)+1> array_impl<T, 0, Rest...>::value;

	template<class T>
	struct stash {
		static_assert(size<T>::value >= 1, "N must be at least 1");
		static constexpr auto& value = array_impl<T, size<T>::value - 1>::value;
	};

    namespace bmi = boost::multi_index;

	template<typename T>
	struct holder {
		typedef typename key_t<T>::type key_type;
        typedef std::pair<key_type, T> pair_type;

        typedef typename bmi::multi_index_container<
              pair_type
            , bmi::indexed_by<
                  bmi::hashed_unique<
                      bmi::tag<tags::key>
                    , bmi::member<
                          pair_type
                        , key_type
                        , &pair_type::first
                      >
                  >
                , bmi::hashed_unique<
                      bmi::tag<tags::value>
                    , bmi::member<
                          pair_type
                        , T
                        , &pair_type::second
                    >
                >
            >
        > type;
	};
}}}}}

namespace Foo {
namespace Utils {
namespace meta {
namespace enums {

    /**
     * An abstract factory for (class) enums.
     * The enum key value pairs get initialy wrapped in some constexpr
     * array and are loaded in a static map on startup. That
     * relatively complex schema allows us to avoid the
     * static initialization fiasco which improves portability.
     */
	template<class T>
	struct factory {
	private:
		static typename details::holder<T>::type* get_map() {
			if (!m_map) {
				m_map.reset(new typename details::holder<T>::type);
				for (auto it : details::stash<T>::value) {
					m_map->insert(typename details::holder<T>::type::value_type(it.first, it.second));
				}
			}

			return m_map.get();
		}

		~factory() {}

	public:
		static typename details::key_t<T>::type key(T t) {
			auto map = factory<T>::get_map();
            auto idx = map->template get<details::tags::value>().find(t);
            auto res = idx->first;
            return res;
        }

		static T value(typename details::key_t<T>::type t) {
			auto map = factory<T>::get_map();
            auto idx = map->template get<details::tags::key>().find(t);
            auto res = idx->second;
            return res;
		}

	private:
		static std::unique_ptr<typename details::holder<T>::type> m_map;
	};
}}}}

#define FOO_DEFINE_ENUM(X)                                                   \
template<>                                                                      \
std::unique_ptr<typename Foo::Utils::meta::enums::details::holder<X>::type>	\
Foo::Utils::meta::enums::factory<X>::m_map =									\
std::unique_ptr<typename Foo::Utils::meta::enums::details::holder<X>::type>();

#define FOO_DECLARE_ELEM(r, data, elem) \
    BOOST_PP_TUPLE_ELEM(3, 0, elem) = BOOST_PP_TUPLE_ELEM(3, 1, elem) ,

#define FOO_FOR_EACH_DECLARE_ENUM(SEQ) \
	BOOST_PP_SEQ_FOR_EACH(FOO_DECLARE_ELEM, ~, FOO_SEQ_DOUBLE_PARENS(SEQ))

#define FOO_DECLARE_CREATOR(r, X, I, elem)						\
	template <> struct create<X, I> {				                \
		static constexpr pair<X>::type load() {	                    \
			return std::make_pair(BOOST_PP_TUPLE_ELEM(2, 1, elem),	\
			X::BOOST_PP_TUPLE_ELEM(2, 0, elem)  );					\
		}															\
	};

#define FOO_FOR_EACH_DECLARE_CREATOR(X, SEQ) \
	BOOST_PP_SEQ_FOR_EACH_I(FOO_DECLARE_CREATOR, X, FOO_SEQ_DOUBLE_PARENS(SEQ))

#define FOO_DECLARE_ENUM_ALL(X, T, SEQ)					\
namespace Foo {                                          \
namespace Utils {                                           \
namespace meta {                                            \
namespace enums {                                           \
namespace details {                                         \
template <>													\
struct key_t<X> {			                                \
	typedef T type;											\
};															\
															\
template<>													\
struct size<X> {			                                \
	static constexpr std::size_t value =					\
	BOOST_PP_SEQ_SIZE(FOO_SEQ_DOUBLE_PARENS(SEQ));		\
};															\
FOO_FOR_EACH_DECLARE_CREATOR(X, SEQ)						\
}}}}}                                                       \
															\
template <typename STREAM>									\
inline STREAM& operator<<(STREAM& os, X elem) {				\
	os << Foo::Utils::meta::enums::factory<X>::key(elem);\
	return (os);											\
}

#define FOO_DECLARE_ENUM(X, SEQ)							\
	FOO_DECLARE_ENUM_ALL(X, const char*, SEQ)

enum yellowKey : int {
	  govt		= 1
	, corp		= 2
	, mtge		= 3
	, mmkt		= 4
	, muni		= 5
	, pfd		= 6
	, equity	= 7
	, cmdty		= 8
	, index		= 9
	, crncy		= 10
	, client	= 11
	, none		= 0
};

FOO_DECLARE_ENUM(
	yellowKey,
	(govt	, "Govt"	)
	(corp	, "Corp"	)
	(mtge	, "Mtge"	)
	(mmkt	, "Mmkt"	)
	(muni	, "Muni"	)
	(pfd	, "Pfd"		)
	(equity	, "Equity"	)
	(cmdty	, "Cmdty"	)
	(index	, "Index"	)
	(crncy	, "Crncy"	)
	(client	, "Client"	)
)

FOO_DEFINE_ENUM(yellowKey);

#include <boost/spirit/include/qi.hpp>

namespace boost { namespace spirit { namespace traits {

    namespace meta = Foo::Utils::meta;

	template<>
	struct transform_attribute<
		yellowKey, std::string, qi::domain> {

		typedef std::string type;

		static type pre(yellowKey a) {
            auto res = meta::enums::factory<yellowKey>::key(a);
            return res;
        }

		static void post(yellowKey& d, type const& v) {
			auto res = meta::enums::factory<yellowKey>::value(v.c_str());
            d = res;
		}

		static void fail(yellowKey& e) { e = yellowKey::none; }
	};
}}}

int main() {
    namespace meta = Foo::Utils::meta;
    auto test = meta::enums::factory<yellowKey>::value("Index");
    auto res = meta::enums::factory<yellowKey>::value("Index");
    return 0;
}
