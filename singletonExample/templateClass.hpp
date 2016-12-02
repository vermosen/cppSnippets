#pragma once

#include "singleton.hpp"

template<typename T>
class myExample : public singleton<myExample<T> >
{
public:
	void myMethod();
private:
	T inner_;
};

template<typename T>
void myExample<T>::myMethod() { return inner_.mySubMethod(); }