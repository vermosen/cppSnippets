#include <iostream>
#include <chrono>

#define QSIZE 0x200000              //~2m int64
#define CNG (cng = 6906969069ULL * cng + 13579)
#define XS (xs ^= (xs << 13), xs ^= (xs >> 17), xs ^= (xs << 43))
#define KISS (B64MWC() + CNG + XS)
 
static std::uint64_t QARY[QSIZE];
static int j;
static std::uint64_t carry;
static std::uint64_t xs;
static std::uint64_t cng;
 
void randk_reset(void)
{
    j = QSIZE - 1;
    carry = 0;
    xs = 362436069362436069ULL;
    cng = 123456789987654321ULL;            // default seed
}
 
std::uint64_t B64MWC(void)
{
    std::uint64_t t, x;
    j = (j + 1) & (QSIZE - 1);
    x = QARY[j];
    t = (x << 28) + carry;
    carry = (x >> 36) - (t < x);
    return (QARY[j] = t - x);
}
 
/* Initialize PRNG with default seed */
/*void randk_seed(void)
{
    Seed QARY[] with CNG+XS: 
    for (std::uint64_t i = 0; i < QSIZE; i++)
        QARY[i] = CNG + XS;
}*/
 
// a kiss 64 bits implementation
class kiss {
public:
    kiss(std::uint64_t seed, int rounds = 0) {
	set_seed(seed);    // set the seed
	warmup(rounds);
    }

    void reset(std::unit64_t seed)
    {
	j = QSIZE - 1;
	carry = 0;
	xs = 362436069362436069ULL;
    } 
    std::uint64_t next() {
	return KISS;
    }
    
private:
    void set_seed(std::uint64_t seed) {
	m_seed ^= seed;
	cng ^= seed;
	xs ^=cng;
	init();
    }
    
    void warmup(int rounds) {
	for (int i = 0; i < rounds; i++) {
	    next();
	}
    }

    void init() {
	for(std::uint64_t i = 0; i < QSIZE; i++) {
	    QARY[i] = CNG + XS;
	}

    }
    
    static std::int64_t m_seed;
};

std::int64_t kiss::m_seed = 0;

int main() {
    typedef std::chrono::high_resolution_clock clock;
    std::uint64_t seed = clock::now().time_since_epoch().count();

    seed = 100;
    const int warmup = 100;
    kiss k(seed, warmup);

    char sep = ' ';
    for(int i = 0; i < 50; i++) {
	std::cout << sep << static_cast<double>(k.next()) /
	    static_cast<double>(std::numeric_limits<std::uint64_t>::max());
	sep = ',';
    }
}
