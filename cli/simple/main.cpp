#using <mscorlib.dll>

using namespace System;

struct myNativeStruct {
	double m_double;
};

public ref struct myManagedStruct			// struct in the sense of C/C++ struct
{
private:
	myNativeStruct * m_native;

public:
	myManagedStruct(double dbl) {
		m_native = new myNativeStruct{ dbl };
	}
	~myManagedStruct() {
		delete m_native;
		m_native = nullptr;
	}

	property double doub
	{
		double get() {
			return m_native->m_double;
		}
		void set(double d) {
			m_native->m_double = d;
		}
	}
};

int main(array<System::String ^> ^args)
{
	System::Console::WriteLine("Hello world");

	myManagedStruct ^ms = gcnew myManagedStruct(3.0);
	ms->doub = 4.0;
	return 0;
}
