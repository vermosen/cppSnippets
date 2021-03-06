
#include <type_traits>

struct foo {};
struct bar {};
struct baz {};

template<class T, class... Types>
struct is_in : std::false_type {};

template<class T, class U, class... Types>
struct is_in<T, U, Types ...>
  : std::integral_constant<bool, std::is_same<T, U>{} || is_in<T, Types... >{} > {};

int main() {

  static_assert(is_in<foo, foo, bar, baz>{}, "test 1 failed!");
  static_assert(is_in<baz, foo, bar, baz>{}, "test 2 failed!");
  static_assert(is_in<foo, bar, baz>{}, "test 3 failed !");
}
