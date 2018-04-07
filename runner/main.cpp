
#include <boost/phoenix.hpp>



struct runner_base {

};

struct runner : runner_base {

	template<typename ... Args>
	void operator()(Args... args) {
		
	}
};

int main()
{
	double t = 0.0;
	runner r;

	r(t);

	return 0;
}