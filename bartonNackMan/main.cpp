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

	// Barton-Nackman
	friend bool operator != (T const& lhs, T const& rhs) {
		return !(lhs == rhs);
	}
};

class child1 : public parent<child1> 
{
	friend parent;
    void interfaceImpl()
    {
        std::cout << "implementation 1" << std::endl;
    }

	// Barton-Nackman
	friend bool operator == (child1 const& lhs, child1 const& rhs) {
		return &lhs == &rhs;
	}
};

class child2 : public parent<child2>
{
	friend parent;
	void interfaceImpl()
	{
		std::cout << "implementation 2" << std::endl;
	}

	// Barton-Nackman
	friend bool operator == (child2 const& lhs, child2 const& rhs) {
		return &lhs == &rhs;
	}
};

int main()
{
	child1 c1, c2;
    c1.interface();  // Prints "Derived implementation"
	child2 c3;
	c3.interface();  // Prints "Derived implementation"

	bool same = (c1 != c2);

    return 0;
}