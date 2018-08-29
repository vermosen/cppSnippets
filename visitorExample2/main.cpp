#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class message {													// base class

	public:

		class visitor {

			public:
				virtual ~visitor() = default;
				virtual bool accept(message *) { return false; }

		};

		virtual ~message() = default;
		virtual bool accept(visitor &) = 0;						// pure virtual

};

template < typename T>											// first basic child
class child : public T {

	public:

		class visitor {

			public:
				virtual ~visitor() = default;
				virtual bool accept(child*) = 0;
		};

		virtual bool accept(message::visitor& av) override {
			auto dv = dynamic_cast<child::visitor*>(std::addressof(av));
			return dv ? dv->accept(this) : av.accept(this);

	}
};

template < typename T>
class childWithCtor : public T {								// second implementation with extended ctor

public:

	// the extended ctor
	childWithCtor(int arg) : arg_(arg) {};

protected:

	int arg_;

public:

	class visitor {

	public:
		virtual ~visitor() = default;
		virtual bool accept(childWithCtor*) = 0;
	};

	virtual bool accept(message::visitor & av) override {

		auto dv = dynamic_cast< childWithCtor::visitor*>(std::addressof(av));
		return dv ? dv->accept(this) : av.accept(this);

	};
};

// the message implementations with various hierarchy levels
class message_A : public child < message > {};
class message_B : public childWithCtor < message > {

	public:
		message_B(int arg) : childWithCtor<message>(arg) {}

};

// buffer
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
class bufferA : public buffer < message_A > {
	virtual void someMethod(message_A* ptr) override { /* do something */ vec_.push_back(ptr); };
};
class bufferB : public buffer < message_B > {
	virtual void someMethod(message_B* ptr) override { /* do something */ vec_.push_back(ptr); };
};

int main() {

	std::chrono::time_point<std::chrono::system_clock> start =		// chrono start
		std::chrono::system_clock::now();

	message * seq[10000];											// the message array

	std::cout	<< "data generated: "
				<< sizeof(seq) / sizeof(seq[0])
				<< std::endl;

	for (auto i = 0; i < 10000; i += 2){							// fill the array

		seq[i] = new message_A;
		seq[i + 1] = new message_B(i);									// use the extended ctor

	}

	bufferA buffer_A;												// instanciate the buffers
	bufferB buffer_B;

	std::chrono::time_point<std::chrono::system_clock> testStart =	// start the real test
		std::chrono::system_clock::now();

	for (message * p : seq) {										// each buffer attempt to store the message

		p->accept(buffer_A);
		p->accept(buffer_B);

	}

	std::vector<message_A*> as = buffer_A.messages();				// the resulting messages by type
	std::vector<message_B*> bs = buffer_B.messages();

	std::chrono::time_point<std::chrono::system_clock> stop =		// stop chrono
		std::chrono::system_clock::now();

	std::chrono::duration<double> testTime  = stop - testStart;
	std::chrono::duration<double> totalTime = stop - start;

	std::cout	<< as.size()										// total number of message catched
				<< " messages of type A"
				<< std::endl
				<< bs.size()
				<< " messages of type B"
				<< std::endl;

	std::cout	<< "test 2 achieved in "							// test summary
				<< totalTime.count()
				<< " s."
				<< std::endl
				<< "dynamic cast achieved in "
				<< testTime.count()
				<< " s"
				<< std::endl;

	system("pause");

}
