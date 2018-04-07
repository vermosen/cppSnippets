#include <chrono>
#include <vector>
#include <iostream>

typedef std::chrono::microseconds duration_type;
typedef std::chrono::system_clock clock_type;
typedef std::chrono::time_point<clock_type> point_type;

int N;
double U;
double U0;
std::vector<double> L;
std::vector<double> D;

inline void compute(
	int n, double a, double b, double c, double d, double e, double f, double g) {

	N = n;
	U = e;
	U0 = b;

	L.resize(N);
	D.resize(N);

	L[0] = 0.0;
	L[N - 1] = 0.0;

	D[0] = a;

	L[1] = c / a;
	D[1] = d - b * c / a;

	for (size_t i = 2; i < static_cast<size_t>(N - 1); ++i) {
		L[i] = c / D[i - 1];
		D[i] = d - L[i] * U;
	}
}

int main() {
	point_type start = clock_type::now();
	compute(10000000, 1.1, 1.2, -1.3, 1.2, 0.5, 0.25, -0.33);
	auto dur = std::chrono::duration_cast<duration_type>(
		clock_type::now() - start);

	std::cout << "took: " << dur.count() << std::endl;

	return 0;
}