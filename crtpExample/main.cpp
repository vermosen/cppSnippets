#include <iostream>

template <typename T>
class parent
{ 
public:
    void interface()
    {
		std::cout << "parent : ";
        static_cast<T*>(this)->interfaceImpl();
    }
};

class child1 : public parent<child1> 
{
	friend parent;
    void interfaceImpl()
    {
        std::cout << "implementation 1" << std::endl;
    }
};

class child2 : public parent<child2>
{
	friend parent;
	void interfaceImpl()
	{
		std::cout << "implementation 2" << std::endl;
	}
};

int main()
{
	child1 c1;
    c1.interface();  // Prints "Derived implementation"
	child2 c2;
	c2.interface();  // Prints "Derived implementation"
    return 0;
}