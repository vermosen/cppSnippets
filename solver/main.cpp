#include <cmath>
#include <limits>

typedef double real;

struct costFunction {
	real operator()(const real& a) const { return a * a; }

	virtual void gradient(real& grad, const real& x) const {
		real eps = finiteDifferenceEpsilon(), fp, fm;
		real xx = x;
		xx += eps;
		fp = this->operator()(xx);
		xx -= 2.0*eps;
		fm = this->operator()(xx);
		grad = 0.5 * (fp - fm) / eps;
	}

	virtual real finiteDifferenceEpsilon() const { return 1e-8; }
};

struct constraint
{
	bool valid(const double& a, const double& d) { return std::abs(a) < 1.0; }
};

struct endCriteria {
public:
	endCriteria(int maxIter) : m_maxIter(maxIter) {}
	enum type {
		maxIterations = 1,
		stationaryPoint = 2,
		stationaryFunctionValue = 3,
		stationaryFunctionAccuracy = 4,
		zeroGradientNorm = 5,
		none = 0
	};

	bool operator()(const int iteration,
		endCriteria::type& ecType,
		const double fold,			// TODO: function state before and after an update
		const double fnew,
		const double gold,
		const double gnew) const
	{
		if (iteration > m_maxIter) return type::maxIterations;
		if (fnew > fold) return type::stationaryPoint;
	}

	int m_maxIter;
};

struct problem {
	double m_initial;
	costFunction m_cost;
	constraint m_constraints;
};

struct method
{
	virtual endCriteria::type optimize(problem& p) = 0;
};

struct bissect : public method
{
	bissect(int max) : m_criteria(max) {}

	virtual endCriteria::type optimize(problem& p)
	{
		int n = 0;
		double f_init = p.m_cost(p.m_initial);
		double f_new = std::numeric_limits<double>::max();
		endCriteria::type retVal;
		for (;;)
		{
			if (m_criteria(n++, retVal, f_init, f_new, 0.0, 0.0))
				return retVal;
		}
	}
private:
	endCriteria m_criteria;
};

int main()
{
	problem p;
	p.m_initial = 0.5;
	method * m = new bissect(100);
	auto res = m->optimize(p);
	return 0;
}