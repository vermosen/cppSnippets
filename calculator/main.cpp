#include <iostream>
#include <vector>

#include <boost/proto/proto.hpp>
#include <boost/typeof/std/ostream.hpp>
using namespace boost;

// Define a placeholder type
template<int I>
struct placeholder {};

// Define the Protofied placeholder terminals
proto::terminal<placeholder<0>>::type const _1 = { {} };
proto::terminal<placeholder<1>>::type const _2 = { {} };

struct calculator_context
	: proto::callable_context< calculator_context const >
{
	// Values to replace the placeholders
	std::vector<double> args;

	// Define the result type of the calculator.
	// (This makes the calculator_context "callable".)
	typedef double result_type;

	// Handle the placeholders:
	template<int I>
	double operator()(proto::tag::terminal, placeholder<I>) const
	{
		return this->args[I];
	}
};

int main() {

	calculator_context ctx;
	ctx.args.push_back(45); // the value of _1 is 45
	ctx.args.push_back(50); // the value of _2 is 50

	// Create an arithmetic expression and immediately evaluate it
	double d = proto::eval((_2 - _1) / _2 * 100, ctx);

	// This prints "10"
	std::cout << d << std::endl;
	return 0;
}
