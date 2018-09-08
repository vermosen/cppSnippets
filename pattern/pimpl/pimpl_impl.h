#pragma once

#include <memory>

namespace Foo {

	/*!
	* Warning! This is a definition file. 
	* See fwd/pimpl.h for the declaration
	* 
	*/
	template<typename T, typename Del>
	pimpl<T, Del>::pimpl() {}

	template<typename T, typename Del>
	template<typename ... Args>
	pimpl<T, Del>::pimpl(Args&& ...args)
		: m_impl(std::unique_ptr<T, Del>(new T { std::forward<Args>(args)... })) {}

	template<typename T, typename Del>
	template<typename Q>
	pimpl<T, Del>::pimpl(Q* ptr)
		: m_impl(std::unique_ptr<T, Del>(ptr)) {

		// does not evaluate in MSVC (but the error raised is correct)
		static_assert(std::is_base_of<T, Q>::value, "incompatible type passed");
	}

	template<typename T, typename Del>
	pimpl<T, Del>::pimpl(pimpl<T, Del>&& o) {
		if (&o != this) {
			m_impl = std::move(o.m_impl);
		}
	}

	template<typename T, typename Del>
	pimpl<T, Del>& pimpl<T, Del>::operator=(pimpl<T, Del>&& o) {
		if (&o != this) {
			m_impl.reset(); 
			m_impl = std::move(o.m_impl);
		}

		return *this;
	}

	template<typename T, typename Del>
	pimpl<T, Del>::~pimpl() {}

	template<typename T, typename Del>
	T* pimpl<T, Del>::operator->() { return m_impl.get(); }

	template<typename T, typename Del>
	T& pimpl<T, Del>::operator*() { return *m_impl.get(); }

}
