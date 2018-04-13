#include <iostream>
#include <utility>

#include <boost/noncopyable.hpp>

struct foo : public boost::noncopyable {
	foo() { m_member = 2; }
	foo(foo&& other) {
		m_member = std::move(other.m_member);
	}

	foo& operator=(foo&& other) {
			
	}

	int m_member;
};

void func(foo f) {
	std::cout << f.m_member << std::endl;
}

int main() {
	foo f;
	func(std::move(f));

	return 0;
}