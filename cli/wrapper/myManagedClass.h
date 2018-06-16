#pragma once

#include "E:\git\cppSnippets\cli\native\myNativeClass.h"

namespace managed_ns {
	public ref class myManagedClass
	{
	public:
		myManagedClass(double dbl) {
			m_native = new ns::myNativeClass();
			m_native->doub(dbl);
		}
		~myManagedClass() {
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
	private:
		ns::myNativeClass* m_native;
	};
}
