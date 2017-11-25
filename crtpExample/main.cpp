#include <iostream>

template <typename T>
class parent
{ 

public:
    void interface()
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
    child d;
    d.interface();  // Prints "Derived implementation"
    return 0;
}