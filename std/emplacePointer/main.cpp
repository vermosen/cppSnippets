
#include <iostream>
#include <map>

class Foo
{
public:
	Foo() {}
	Foo(double z) : m_z(z)
	{}

	double m_z;
};

int main()
{

	std::map<int, Foo*> my_map({
		{1, new Foo(3.14)},
		{2, new Foo(2.718)}
		});

	int i = 7;
	auto iter = my_map.find(i);
	try {
		if (iter == my_map.end()) {
			std::cout << i << " doesn't exist in my map!" << std::endl;
		}

		std::cout << i << " => " << iter->second->m_z << std::endl;
		std::cout << "No execption thrown" << std::endl;

	}
	catch (...) {
		std::cout << "caught exception" << std::endl;
	}
}
