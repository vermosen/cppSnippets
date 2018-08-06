#include <iostream>
#include <string>

#include <boost/mpl/vector.hpp>

#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace test {

    template <typename T>
    struct table;

    template <typename T> struct _1;
    template <typename T> struct _2;


    struct my_struct {
        int m_1;
        double m_2;
    };

    template <>
    struct table<my_struct> {
        typedef typename boost::mpl::vector<
              typename _1<my_struct>
            , typename _2<my_struct>
        >::type meta_info;
    };

    template <> struct _1 <my_struct> {
        static constexpr const char* field_name = "m1";
    };

    template <> struct _2 <my_struct> {
        static constexpr const char* field_name = "m2";
    };
}


struct F {
    typedef const std::string& result_type;
    template <class Seq1, class Seq2>
    auto operator()(Seq1 s1, Seq2 s2) const {
        return s2;
    }
};

struct print {
    template <typename T>
    void operator()(const T& t) const {
        std::cout << t;
    }
};

int main() {
    auto values = boost::fusion::vector<int, double>(1, 3.0);
    auto fields = boost::fusion::vector<std::string, std::string>("hello", "world");
    auto res = boost::fusion::transform(values, fields, F());
    boost::fusion::for_each(res, print());
    return 0;
}
