#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/include/as_vector.hpp>

struct test {
    std::int64_t m_t;
    double n_t;
};

namespace cassandra { namespace extension {

    template<typename Seq, int N>
    struct struct_field_name;

    template<typename Seq, int N>
    struct is_clustering_key;

    template<typename Seq, int N>
    struct is_partitioning_key;
}}

#ifndef USE_MACRO
//#define USE_MACRO
#endif

#ifdef USE_MACRO
BOOST_FUSION_ADAPT_STRUCT(test,
    (std::int64_t, m_t)
    (double, n_t)
)
#else

namespace boost {
namespace fusion {
namespace traits {

    template<>
    struct tag_of<test> {
        typedef struct_tag type;
    };

    template<>
    struct tag_of<test const> {
        typedef struct_tag type;
    };
}

namespace extension {

    template<>
    struct access::struct_member< test, 0 > {
        typedef std::int64_t attribute_type;
        typedef attribute_type type;

        // return const reference to the first argument
        template<typename Seq>
        struct apply {

            typedef typename add_reference<
                typename mpl::eval_if<
                        is_const<Seq>
                    , add_const<attribute_type>
                    , mpl::identity<attribute_type>
                >::type
            >::type type;

            static type call(Seq& seq) {
                return seq.m_t;
            }
        };
    };

    template<>
    struct access::struct_member< test, 1 > {
        typedef double attribute_type;
        typedef attribute_type type;

        // return const reference to the first argument
        template<typename Seq>
        struct apply {

            typedef typename add_reference<
                typename mpl::eval_if<
                is_const<Seq>
                , add_const<attribute_type>
                , mpl::identity<attribute_type>
                >::type
            >::type type;

            static type call(Seq& seq) {
                return seq.n_t;
            }
        };
    };

    template<>
    struct struct_member_name<test, 0> {
        typedef char const* type;
        static type call() { return "m_t"; }
    };

    template<>
    struct struct_member_name<test, 1> {
        typedef char const* type;
        static type call() { return "n_t"; }
    };

    template<>
    struct struct_size<test> : mpl::int_<2> {};

    template<> struct struct_is_view<test> : mpl::false_ {};
}}

namespace mpl {
    template<typename>
    struct sequence_tag;

    template<> struct sequence_tag<test> {
        typedef fusion::fusion_sequence_tag type;
    };

    template<>
    struct sequence_tag<test const> {
        typedef fusion::fusion_sequence_tag type;
    };
}}

namespace cassandra { namespace extension {

    // added field name
    template<>
    struct struct_field_name<test, 0> {
        typedef char const* type;
        static type call() { return "id"; }
    };

    template<>
    struct is_clustering_key<test, 0> : boost::mpl::true_ {};

    template<>
    struct is_partitioning_key<test, 0> : boost::mpl::false_ {};

    template<>
    struct struct_field_name<test, 1> {
        typedef char const* type;
        static type call() { return "value"; }
    };

    template<>
    struct is_clustering_key<test, 1> : boost::mpl::true_ {};

    template<>
    struct is_partitioning_key<test, 1> : boost::mpl::false_ {};
}}

#endif

int main() {

    test t = { 1 };
    auto v = boost::fusion::as_vector(t);
    return 0;
}
