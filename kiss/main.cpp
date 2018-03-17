#include <iostream>
#include <stdint.h>

struct  kiss {
    inline std::uint64_t next() {
	return m_t =
	    (m_t = (m_x << 58) + m_c,
	         m_c = (m_x >> 6),
	         m_x += m_t,
	         m_c += (m_x < m_t),
	         m_x) +
	    (m_y ^= (m_y << 13),
	         m_y ^= (m_y >> 17),
	         m_y ^= (m_y << 43)) +
	    (m_z = 6906969069 * m_z + 1234567);
    }

    static std::uint64_t m_x;
    static std::uint64_t m_c;
    static std::uint64_t m_z;
    static std::uint64_t m_y;
    static std::uint64_t m_t;
};

std::uint64_t kiss::m_x = 1234567890987654321;
std::uint64_t kiss::m_c = 123456123456123456 ;
std::uint64_t kiss::m_z = 1066149217761810   ;
std::uint64_t kiss::m_y = 362436362436362436 ;
std::uint64_t kiss::m_t = 0;

int main() {

    kiss k;
    for(int i = 0; i < 100000000; i++) {
	kiss::m_t = k.next();
	if (i < 5) std::cout << kiss::m_t << std::endl;
    }
	
    if(kiss::m_t == 1666297717051644203) {
	std::cout << "100 million uses of KISS OK" << std::endl;
	return 0;
    } else {
	std::cout << "Fail: t==" << kiss::m_t << std::endl;
	return -1;
    }
}
