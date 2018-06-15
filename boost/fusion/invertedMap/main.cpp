
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <string>
#include <iostream>
#include <typeinfo>

using namespace boost::fusion;


template<class Pred,class Fun>
 struct filter {
    Pred pred_;
    Fun& fun_;

    filter(Pred p, Fun& f)
        : pred_(p)
        , fun_(f)
    {}

    template<class Pair>
    void operator()(Pair& pair) const {
        if (pred_(pair.second))
            fun_(pair);
    }
 };

 template<class Pred,class Fun>
 filter<Pred,Fun> make_filter(Pred p, Fun& f) {
    return filter<Pred,Fun>(p, f);
 }

 typedef map
    < pair<int, char>
    , pair<double, std::string>
 > map_type;

 struct fun {
    template<class First,class Second>
    void operator()(pair<First,Second>& t) const {
        std::cout
            << typeid(First).name() << std::endl
            << typeid(Second).name() << ":" << t.second << std::endl;
    }
 };

 struct mypred {
    template<class T>
    bool operator()(T const&) const {
        return false;
    }

    bool operator()(char c) const {
        return c=='X';
    }
};

int main(int argc, char** argv) {

    map_type m(
        make_pair<int>('X'),
        make_pair<double>("Men")
    );

    for_each(m, make_filter(mypred(),fun()));

    return 0;
}
