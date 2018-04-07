#include <array>
#include <vector>

template<typename T>
struct multilinear_adapter;

struct point
{
	double x_s;
};

template<>
struct multilinear_adapter<point>
{
	static const int dimension = 1;
	typedef point in_type;
	typedef double out_type;

	inline std::array<out_type, dimension> adapt(const point & in)
	{
		return std::array<out_type, dimension> {in.x_s};
	}
};

int main()
{
	point p{ 1.0 };


	return 0;
}