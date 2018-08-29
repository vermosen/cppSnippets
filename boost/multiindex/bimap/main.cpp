#include <string>
#include <iostream>

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

template <typename T>
struct key {};

template <typename T>
struct value {};

namespace bmp = boost::bimaps;

template <typename T>
using map_type = bmp::bimap
<
    bmp::unordered_set_of<bmp::tagged<const char*, key<T>>>,
    bmp::unordered_set_of<bmp::tagged<T, value<T>>>
>;

template <typename T>
using pair = typename map_type<T>::value_type;

enum class my_enum {
      one = 1
    , two = 2
    , three = 3
};

static map_type<my_enum> m_map = map_type<my_enum>();

int main()
{
    m_map.insert(pair<my_enum>("one", my_enum::one));
    m_map.insert(pair<my_enum>("two", my_enum::two));
    m_map.insert(pair<my_enum>("three", my_enum::three));

    for (map_type<my_enum>::map_by<key<my_enum>>::const_iterator
        i = m_map.by<key<my_enum>>().begin();
        i != m_map.by<key<my_enum>>().end(); i++) {

        std::cout
            << "(" << i->get<key<my_enum>>() << ") "
            << std::endl;
    }

    map_type<my_enum>::map_by<value<my_enum>>::const_iterator it = m_map.by<value<my_enum>>().find(my_enum::two);
    if (it == m_map.by<value<my_enum>>().end()) {
        // name does not exists
    }
    else {
        std::cout << it->get<key<my_enum>>() << std::endl;
    }

    return 0;
}
