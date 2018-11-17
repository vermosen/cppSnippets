#pragma once

class observable;

class observer {
public:

	observer();

	void bind(observable& o);
	void update(double d);
	double m_value;
};
