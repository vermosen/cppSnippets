#include <limits>
#include <random>
#include <iostream>
#include <list>
#include <set>
#include <ctime>
#include <iomanip>

using data_type = std::int64_t;

template <typename Child>
struct sampler {                                                    // CRTP

    sampler()
        : m_dist(std::numeric_limits<data_type>::min()
            , std::numeric_limits<data_type>::max())
        , m_rg(m_seed) {

    }

    void operator()() {
        std::clock_t start = std::clock();                          // cpu clock
        std::cout << "starting test "
            << Child::m_label
            << " with  parameters "
            << "szops=" << szops
            << ",nops=" << nops
            << std::endl;

        data_type dummy = static_cast<Child&>(*this).invoke();      // to avoid elision, we need to return a dummy value here...
        std::cout
            << "returned (" << dummy << ") in "
            << std::fixed << std::setprecision(4)
            << 1000.0 * (std::clock() - start) / CLOCKS_PER_SEC
            << " milliseconds." << std::endl;
    }

    std::seed_seq m_seed = { 0 };                                   // for a fair comparison, we pin the sequence
    std::uniform_int_distribution<data_type> m_dist;
    std::default_random_engine  m_rg;
    const std::size_t szops = 5e4;                                  // the target size of the container
    const std::size_t nops  = 1e6;                                  // the number of push/pop to operate
};

template <typename T>
struct test;

template <>
struct test<std::list<data_type>>
    : public sampler<test<std::list<data_type>>> {

    data_type invoke() {

        data_type dummy = m_dist(m_rg);
        m_container.push_front(dummy);                              // push one to avoid dealing with the empty container

        for (size_t i = 0; i < szops; i++) {                        // phase 1: reach a certain size
            auto data = m_dist(m_rg);
            bool seek = false;

            for (auto it = m_container.begin(); it != m_container.end(); it++) {
                if (data < *it) {
                    m_container.insert(it, data);                   // insert and break
                    seek = true;
                    break;
                }
            }

            if (!seek) {
                m_container.push_back(data);
            }

            dummy += data;
        }

        for (size_t i = 0; i < nops; i++) {                         // phase 2: alternate push/pop

            dummy += *m_container.begin();
            m_container.pop_front();

            auto data = m_dist(m_rg);
            bool seek = false;

            for (auto it = m_container.begin(); it != m_container.end(); it++) {
                if (data < *it) {
                    m_container.insert(it, data);                   // insert and break
                    seek = true;
                    break;
                }
            }

            if (!seek) m_container.push_back(data);
            dummy += data;
        }

        return dummy;
    }

    std::list<data_type> m_container;

    static constexpr const char* m_label = "[list]";
};

template <>
struct test<std::multiset<data_type>>
    : public sampler<test<std::multiset<data_type>>> {

    data_type invoke() {

        data_type dummy = m_dist(m_rg);
        m_container.insert(dummy);                                  // push one to avoid dealing with the empty container

        for (size_t i = 0; i < szops; i++) {                        // phase 1: reach a certain size
            auto data = m_dist(m_rg);
            dummy += data;
            m_container.insert(data);
        }

        for (size_t i = 0; i < nops; i++) {                         // phase 2: alternate push/pop

            auto first = m_container.begin();
            dummy += *first;
            m_container.erase(first);

            auto data = m_dist(m_rg);
            m_container.insert(data);
            dummy += data;
        }

        return dummy;
    }

    std::multiset<data_type> m_container;
    static constexpr const char* m_label = "[multiset]";
};

int main()
{
    { test<std::list    <data_type>> test1; test1(); }              // list
    { test<std::multiset<data_type>> test2; test2(); }              // multiset

    return EXIT_SUCCESS;
}
