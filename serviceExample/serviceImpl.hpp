#pragma once
#ifndef SERVICE_IMPL_HPP_
#define SERVICE_IMPL_HPP_

#include <boost/thread.hpp>

#include "service.hpp"

namespace dtcc
{
	class serviceImpl : public service<serviceImpl>
	{
	public:
		serviceImpl() {}

		void setPreferences(int pref)
		{
			child().setPreferenceImpl(pref);
		}

		void onStartImpl(DWORD dwArgc, LPSTR * pszArgv)
		{
			boost::thread * thr = new boost::thread(boost::bind(&serviceImpl::startWorkers, this));
			thr->detach();
		}
		void onStopImpl() { run_ = false; boost::this_thread::sleep(boost::posix_time::milliseconds(5000)); }
		void onPauseImpl() { run_ = false; }
		void onContinueImpl() { run_ = true; }
		void onShutdownImpl() { run_ = false; }

		static std::string name() { return "dtccService"; };
		bool canStop() { return true; };
		bool canShutdown() { return false; };
		bool canPauseContinue() { return false; };

	private:

		void setPreferenceImpl(int pref)
		{
			pref_ = pref;
		}

		void startWorkers()
		{
			if (run_)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
			}
		}

		bool run_;
		int pref_;
	};
}

#endif