#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class message {
	
	public: 

		class visitor { 

			public: 
				virtual ~visitor() = default; 
				virtual bool accept(message *) { return false; }
		
		};

		virtual ~message() = default;
		virtual bool accept(visitor &) = 0;						// pure virtual

};

template < typename T> 
class child : public T {
	
	public: 

		class visitor { 
		
			public:
				virtual ~visitor() = default; 
				virtual bool accept(child*) = 0;
		};

		virtual bool accept(message::visitor & av) override {

			auto dv = dynamic_cast< child::visitor*>(std::addressof(av));
			return dv ? dv->accept(this) : av.accept(this);

	}
};

// the message implementations with various hierarchy levels
class message_B : public child < message   > {};
class message_C : public child < message   > {};
class message_D : public child < message_B > {};
class message_E : public child < message_C > {};
class message_F : public child < message_D > {};

// a first visitor implementation designed for counting messages
template < typename T >
class counting_visitor : public message::visitor {

	public: 

		virtual bool accept(message* p) { return dynamic_cast< T* >(p) ? ++cnt : false; }
		operator int() const { return cnt; }
	
	private: 

		int cnt = 0;

};

// a second visitor implementation for buffering stuff
template < typename T > 
class buffer : public message::visitor {

	public:

		virtual bool accept(message* p) {

			if (T * ptr = dynamic_cast<T*>(p)){
				
				// call some method here
				someMethod(ptr);				
				return true;

			} else 
				return false;
	
		}
	
		std::vector<T*> & messages(){ return vec_; }

	protected:

		virtual void someMethod(T*) = 0;
		std::vector<T*> vec_;

};

// actual implementation classes
class bufferB : public buffer < message_B > {
	virtual void someMethod(message_B* ptr) override { /* do something */ vec_.push_back(ptr); };
};
class bufferC : public buffer < message_C > {
	virtual void someMethod(message_C* ptr) override { /* do something */ vec_.push_back(ptr); };
};
class bufferD : public buffer < message_D > {
	virtual void someMethod(message_D* ptr) override { /* do something */ vec_.push_back(ptr); };
};

int main() {

	std::chrono::time_point<std::chrono::system_clock> start =		// chrono start
		std::chrono::system_clock::now();

	message * seq[10000];											// the message array

	std::cout	<< "data generated: "
				<< sizeof(seq) / sizeof(seq[0])
				<< std::endl;

	for (auto i = 0; i < 10000; i += 5){							// fill the array
	
		seq[i]     = new message_B;
		seq[i + 1] = new message_C;
		seq[i + 2] = new message_D;
		seq[i + 3] = new message_E;
		seq[i + 4] = new message_F;
	
	}
	
	bufferB buffer_B;												// instanciate the buffers 
	bufferC buffer_C;
	bufferD buffer_D;

	std::chrono::time_point<std::chrono::system_clock> testStart =	// start the real test
		std::chrono::system_clock::now();

	for (message * p : seq) {										// each buffer attempt to store the message
	
		p->accept(buffer_B); 
		p->accept(buffer_C); 
		p->accept(buffer_D); 
	
	}

	std::vector<message_B*> bs = buffer_B.messages();				// the resulting messages by type
	std::vector<message_C*> cs = buffer_C.messages();
	std::vector<message_D*> ds = buffer_D.messages();

	std::chrono::time_point<std::chrono::system_clock> stop =		// stop chrono
		std::chrono::system_clock::now();

	std::chrono::duration<double> testTime  = stop - testStart;
	std::chrono::duration<double> totalTime = stop - start;

	std::cout	<< bs.size()										// total number of message catched
				<< " messages of type B"
				<< std::endl
				<< cs.size()
				<< " messages of type C"
				<< std::endl
				<< ds.size()
				<< " messages of type D"
				<< std::endl;

	std::cout	<< "test 1 achieved in "							// test summary
				<< totalTime.count()
				<< " s."
				<< std::endl
				<< "dynamic cast achieved in "
				<< testTime.count()
				<< " s"
				<< std::endl;

	system("pause");

}