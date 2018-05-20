#include <iostream>
#include <vector>

#include <boost/proto/proto.hpp>
#include <boost/typeof/std/ostream.hpp>
using namespace boost;

// Define a placeholder type

namespace table {
	struct first_name {};
	struct last_name {};
	struct address {};

}
	template<typename T>
	struct field {};

	// Define the Protofield placeholder terminals
	proto::terminal<field<table::first_name>>::type const first_name_t = { {} };
	proto::terminal<field<table::last_name>>::type const last_name_t = { {} };
	proto::terminal<field<table::address>>::type const address_t = { {} };


struct cql_context
	: proto::callable_context<cql_context const>
{
	// Values to replace the placeholders
	std::vector<std::string> args;

	// Define the result type of the calculator.
	// (This makes the calculator_context "callable".)
	typedef double result_type;

	// Handle the placeholders:
	template<Int I>
	double operator()(proto::tag::terminal, field<T>) const {
		return this->args[I];
	}
};

int main() {

	cql_context ctx;
	ctx.args.push_back("bob"); // the value of _1 is 45
	ctx.args.push_back("dole"); // the value of _2 is 50
	ctx.args.push_back("369 W 126 street"); // the value of _2 is 50

	// Create an arithmetic expression and immediately evaluate it
	double d = proto::eval(first_name_t + last_name_t + address_t, ctx);

	// This prints "10"
	std::cout << d << std::endl;
	return 0;
}
