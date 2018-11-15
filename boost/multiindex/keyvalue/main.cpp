#include <string>
#include <iostream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

enum class test : char {
      foo = 0
    , bar = 1
    , baz = 2
};

namespace tags{
    struct key {};
    struct value {};
}
namespace bmi = boost::multi_index;

typedef const char* key_type;
typedef test value_type;

using map_type = boost::multi_index_container<
    std::pair<key_type, value_type>
    , bmi::indexed_by<
    bmi::ordered_unique<
    bmi::tag<tags::key>
    , bmi::member<std::pair<key_type, value_type>, key_type, &std::pair<key_type, value_type>::first>
    >
    , bmi::ordered_unique<
    bmi::tag<tags::value>
    , bmi::member<std::pair<key_type, value_type>, value_type, &std::pair<key_type, value_type>::second>
    >
    >
>;

int main()
{
    map_type es;

    es.insert(std::pair<const char *, test>("foo", test::foo));
    es.insert(std::pair<const char *, test>("bar", test::bar));
    es.insert(std::pair<const char *, test>("baz", test::baz));

    auto res = es.get<tags::key>().find("foo")->second;

    return 0;
}
