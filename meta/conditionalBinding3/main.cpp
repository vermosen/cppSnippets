#include <tuple>

struct trade {};
struct quote {};
struct update {};

namespace calculators {
  class closure {
  public:
    void handle(trade& ) {}
    void handle(quote& ) {}
    void handle(update&) {}
  };
}

template <typename Child, typename ... Types>
class calculator;

template <typename Child, typename First, typename ... Rest>
class calculator<Child, First, Rest ...> : public calculator<Child, Rest...> {
public:
  using calculator<Child, Rest...>::handle;

  void handle(First& t) {
    static_cast<Child&>(*this).apply(t);
  }
};

template <typename Child, typename Last>
class calculator<Child, Last> : public calculators::closure {
public:
  using calculators::closure::handle;

  void handle(Last& t) {
    static_cast<Child&>(*this).apply(t);
  }
};

class myCalculator : public calculator<myCalculator, trade, quote> {
public:
  void apply(trade& tr) { m_status *= false; }
  void apply(quote& qt) { m_status *= false; }

  bool m_status;
};

int main() {

  myCalculator calc;

  trade t; quote q; update u;

  calc.handle(t);
  calc.handle(q);
  calc.handle(u);
}
