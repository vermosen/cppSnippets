
#include <iostream>
#include <type_traits>

struct foo {};
struct bar {};
struct baz {};

template<class T, class... Types>
struct is_in : std::false_type {};

template<class T, class U, class... Types>
struct is_in<T, U, Types ...>
  : std::integral_constant<bool, std::is_same<T, U>{} || is_in<T, Types... >{} > {};


template <typename T, typename ... Types>
struct checker {
  static constexpr bool value = is_in<T, Types ... >::value;
};

template <bool>
struct wrapper_impl {
  static void invoke() { std::cout << "hello" << std::endl; }
};

template <>
struct wrapper_impl<false> {
  static void invoke() { std::cout << "world!" << std::endl; }
};

template <typename T, typename ... Types>
struct wrapper : public wrapper_impl<checker<T, Types ... >::value> {
  using wrapper_impl<checker<T, Types ... >::value>::invoke;
};

int main() {

  wrapper<foo, foo, bar>::invoke();    // hello
  wrapper<bar, foo, bar>::invoke();    // hello
  wrapper<baz, foo, bar>::invoke();    // world!
}
