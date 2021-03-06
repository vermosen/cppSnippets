
#include <iostream>
#include <boost/proto/proto.hpp>

using namespace boost;

// grammar
struct add_ints_grammar :
    proto::plus < 
          proto::terminal<int>
        , proto::terminal<int>
    > {};

template <typename Expr>
void check_expr(const Expr&) {

    static_assert(
        proto::matches<
              Expr
            , add_ints_grammar
        >::type::value, "");
}

// transform
struct evaluate_plus : proto::callable {
    using result_type = int;
    result_type operator()(int a, int b) const {
        return a + b;
    }
};

struct add_ints_transform : 
    proto::when<
          add_ints_grammar
        , evaluate_plus(
              proto::_value(proto::_left)
            , proto::_value(proto::_right)
        )
    > {};

int main()
{
    int i = 1, j = 2;
    proto::literal<int&> i_term(i), j_term(j);
    auto expr = i_term + j_term;
    check_expr(expr);
    proto::display_expr(expr);
    const int res = add_ints_transform()(expr);

    std::cout << res << std::endl;
    return 0;
}
