/*
*
* signalExample
* Copyright (C) 2015 Jean-Mathieu Vermosen
*
*/

#ifndef quote_hpp
#define quote_hpp

#include "signals/sender.hpp"

class quoteScenario : public sender<double> {

public:

	quoteScenario(const std::vector<double> & values) : values_(values) {};
	double values() const { return values_; };
	
	void next() {

		values_ = values;
		sender::operator()(value_);

	}

private:
	
	std::vector<double> values_;

};

#endif