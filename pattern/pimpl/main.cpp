#include <iostream>

#include "pimpl_h.h"
#include "pimpl_impl.h"

namespace impl {
	class _base {
	public:
		_base(bool t) : m_t(t) {}
		virtual void print() { std::cout << "hello" << std::endl; }

	private:
		bool m_t;
	};

	class _derived : public _base {
	public:
		_derived(bool t) : _base(t) {}
		void print() { std::cout << "world" << std::endl; }
	};

	class _other {
	public:
		_other(bool t) : m_t(t) {}
		void print() { std::cout << "world" << std::endl; }

	private:
		bool m_t;
	};
}

class base {
public:
	base(bool t) {
		m_impl = Foo::pimpl<impl::_base>(new impl::_base(t));
	}
	void print() { m_impl->print(); }

protected:
	Foo::pimpl<impl::_base> m_impl;
};

class derived : public base {
public:
	derived(bool t) : base(t) {
		m_impl = Foo::pimpl<impl::_base>(new impl::_other(t));
	}
};

int main() {
	base* b = new base(true);
	b->print();
	delete b;

	base* d = new derived(true);
	d->print();
	return 0;
}
