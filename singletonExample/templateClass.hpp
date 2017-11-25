#pragma once

#include "singleton.hpp"

template<typename T>
class myExample : public singleton<myExample<T> >
{
public:
	static void myMethod();
	void myFriendMethod();
private:
	static T inner_;
};

template<typename T>
void myExample<T>::myMethod() { std::cout << "hello world !" << std::endl; }

template<typename T>
void myExample<T>::myFriendMethod() { inner_.mySubMethod(); }