

struct test {

	int m_a;
	double m_b;
};

template<typename ... Args>
test stuff(Args ... args) {
	test t = { args... };
	return t;
}

int main() {

	auto res = stuff<int, double>(3, 2.0);

	return 0;
}