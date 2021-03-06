
struct trade  {};
struct quote  {};
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

// calculator begin
class myCalculator : public calculator<myCalculator> {
public:
  bool m_status;
};

template <>
struct handler<myCalculator, trade> {
  static void apply(myCalculator& calc, trade& tr) {
    calc.m_status *= false;
  }
};

template <>
struct handler<myCalculator, quote> {
  static void apply(myCalculator& calc, quote& qt) {
    calc.m_status *= false;
  }
}; // calculator end

int main() {

  myCalculator calc;

  trade t; quote q; update u;

  calc.handle(t);
  calc.handle(q);
  calc.handle(u);
}
