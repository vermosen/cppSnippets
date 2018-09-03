#pragma once

#include <memory>

namespace Foo {

    /*!
     * Brief implementation of the p_impl idiom.
	 * see https://herbsutter.com/gotw/_101/
     */
	template <
          class T
        , class Del = std::default_delete<T>
    >
	class pimpl {

	public:

		pimpl();

		template<typename ...Args> 
		pimpl(Args&& ...);

		template<typename Q>
		pimpl(Q* ptr);

		pimpl(pimpl<T, Del>&& other);

		pimpl<T, Del>& operator=(pimpl<T, Del>&& other);

		//pimpl();
		~pimpl();

		T* operator->();
		T& operator*();

	private:
		std::unique_ptr<T, Del> m_impl;
	};
}
