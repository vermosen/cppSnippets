#include <iostream>
#include <vector>

namespace meta {
namespace details {

    class visitor {
    public:
        virtual ~visitor() {};
    };

    template <class T>
    class atom {
    public:
        virtual void visit(T&) = 0;
    };
}

    template <class Base>
    class visitable {
    public:
        template <typename Derived>
        void accept(Derived& visitor) {
            visitor.visit(static_cast<Base&>(*this));
        }
    };

    template <class ... Types>
    class visitor : public details::visitor, public details::atom<Types>... {};
}

/** example of use */
class quoteA; class quoteB;

class recorderA : public meta::visitable<recorderA> {
public:
    void add(quoteA& a) { std::cout << "recorderA" << std::endl; }
};

class recorderB : public meta::visitable<recorderB> {
public:
    void add(quoteB& b) { std::cout << "recorderB" << std::endl; }
};

class quote : public meta::visitor<recorderA, recorderB> {
public:
    virtual void visit(recorderA& c) = 0;
    virtual void visit(recorderB& c) = 0;
};

class quoteA : public quote {
public:
    virtual void visit(recorderA& c) { std::cout << "quoteA >> "; c.add(*this); }
    virtual void visit(recorderB& c) { /* do nothing */ }
};

class quoteB : public quote {
public:
    virtual void visit(recorderA& c) { /* do nothing */ }
    virtual void visit(recorderB& c) { std::cout << "quoteB >> "; c.add(*this); }
};

int main(int argc, char **argv) {

    std::vector<quote*> quotes = { new quoteA, new quoteB };

    recorderA rec1;
    recorderB rec2;

    for (auto& it : quotes) {
        rec1.accept<quote>(*it);
        rec2.accept<quote>(*it);
    }

    return 0;
}
