#pragma once

namespace ns {
	class myNativeClass
	{
	public:
		myNativeClass();
		virtual ~myNativeClass();

		double m_double;

		inline double doub() const { return m_double; }
		inline void doub(double v) { m_double = v; }
	};
}


