#include <iostream>

class parent_base {
public:
	virtual ~parent_base() {};
	virtual void interface() = 0;
};

template <typename T>
class parent : public parent_base
{ 
	virtual ~parent<T>() {};
public:
    void interface() override
    {
        static_cast<T*>(this)->interfaceImpl();
    }
};

class child : public parent<child> 
{
    friend parent;
private:
    void interfaceImpl()
    {
        std::cout << "Derived implementation\n";
    }
};

int main()
{
	child c();
	parent_base& d(c);
    d.interface();  // Prints "Derived implementation"
    return 0;
}