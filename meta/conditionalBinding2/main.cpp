#include <tuple>

struct trade {};
struct quote {};
struct update {};

template <typename T, typename K>
struct handler {
  static void apply(T&, K&) {}
};

template <typename Child>
class calculator {
public:
  template <typename T>
  void handle(T& tr) {
    handler<Child, T>::apply(static_cast<Child&>(*this), tr);
  }
};

class myCalculator : public calculator<myCalculator> {
public:
  using calc_types = std::tuple<trade, quote>;

  void apply(trade& tr) { m_status *= false; }
  void apply(quote& qt) { m_status *= false; }

  bool m_status;
};

template <>
struct handler<myCalculator, trade> {
  static void apply(myCalculator& calc, trade& tr) {
    calc.apply(tr);
  }
};

template <>
struct handler<myCalculator, quote> {
  static void apply(myCalculator& calc, quote& qt) {
    calc.apply(qt);
  }
};

int main() {

  myCalculator calc;

  trade t; quote q; update u;

  calc.handle(t);
  calc.handle(q);
  calc.handle(u);
}
