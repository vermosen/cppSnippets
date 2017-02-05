#pragma once
#include <iostream>

template <class T>
class X 
{ 

public:
	void interface()
	{
		static_cast<T*>(this)->interface();
	}
};

class A : public X<A> 
{
	friend X;
private:
	void interface()
	{
		std::cout << "Derived implementation\n";
	}
};

int main()
{
	A d;
	d.X::interface();  // Prints "Derived implementation"
	return 0;
}