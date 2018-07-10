#include <iostream>

template <class Impl, class Base = void>
class curiouslyRecurring {
protected:
	curiouslyRecurring() {}
	virtual ~curiouslyRecurring() {}

	inline Impl& impl() {
		return static_cast<Impl&>(*this);
	}

	inline const Impl& impl() const {
		return static_cast<const Impl&>(*this);
	}
};

template <typename T>
class parent : public curiouslyRecurring<T>
{
public:
    void interface()
    {
		std::cout << "parent" << std::endl;
		this->curiouslyRecurring<T>::impl().interfaceImpl();
    }
};

template <typename T>
class mezzo : public parent<mezzo<T>>, public curiouslyRecurring<T>
{
	friend parent;
    void interfaceImpl()
    {
        std::cout << "mezzo" << std::endl;
		this->curiouslyRecurring<T>::impl().interfaceImpl();
    }
};

class child : public mezzo<child>
{
	friend mezzo<child>;
	void interfaceImpl()
	{
		std::cout << "child" << std::endl;
	}
};

int main()
{
	child c1;
    c1.interface();  // Prints "Derived implementation"
    return 0;
}
