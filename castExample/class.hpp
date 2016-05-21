#include <iostream>

class base1 {

public: 
	virtual void doStuff1(){
		
		std::cout << "I am of type 1" << std::endl;
		
	};

	virtual ~base1() {};
};

class base2 {

public:
	virtual void doStuff2(){

		std::cout << "I am of type 2" << std::endl;

	};

	virtual ~base2() {};

};

class child : public base1, public base2 {

public:
	virtual void doStuff1(){

		std::cout << "I am of type 1...but also of type 2 !" << std::endl;

	};

	virtual void doStuff2(){

		std::cout << "I am of type 2...but also of type 1 !" << std::endl;

	};

};
