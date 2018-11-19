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
		curiouslyRecurring<T>::impl().interface();
    }
};

class child : public parent<child>
{
public:
    using parent::interface;

    friend parent<child>;
protected:
	void interface()
	{
		std::cout << "child" << std::endl;
	}
};

int main()
{
    parent<child>& c1  = child();
    c1.interface();
    return 0;
}
