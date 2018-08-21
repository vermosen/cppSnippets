#include <string>
#include <iostream>

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

enum class my_enum {
      one   = 1
    , two   = 2
    , three = 3
};

struct key {};
struct value {};

namespace bmp = boost::bimaps;

typedef bmp::bimap
<
    bmp::unordered_set_of<bmp::tagged<const char *, key>>,
    bmp::unordered_set_of<bmp::tagged<my_enum, value>>

> results_bimap;

typedef results_bimap::value_type pair;

int main()
{
    results_bimap results;
    results.insert(pair("one", my_enum::one));
    results.insert(pair("two", my_enum::two));
    results.insert(pair("three", my_enum::three));

    for (results_bimap::map_by<key>::const_iterator i = results.by<key>().begin(); i != results.by<key>().end(); i++) {

        std::cout
            << "(" << i->get<key>() << ") "
            << std::endl;
    }

    results_bimap::map_by<value>::const_iterator it = results.by<value>().find(my_enum::two);
    if (it == results.by<value>().end()) {
        // name does not exists
    }
    else {
        std::cout << it->get<key>() << std::endl;
    }

    return 0;
}
