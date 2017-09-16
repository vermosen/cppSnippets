#pragma once
#ifndef SERVICE_IMPL_HPP_
#define SERVICE_IMPL_HPP_

#include <boost/thread.hpp>

#include "service.hpp"
#include "debugger.hpp"

namespace dtcc
{
	class serviceImpl : public service
	{
		friend service;
	public:
		serviceImpl(int pref) : service("dtccService", true, false, false)
		{
			pref_ = pref;
		}

	protected:
		virtual void onStart(DWORD dwArgc, LPSTR * pszArgv)
		{
			run_ = true;
			boost::thread * thr = new boost::thread(boost::bind(&serviceImpl::startWorkers, this));
			thr->detach();
		}
		virtual void onStop() { run_ = false; boost::this_thread::sleep(boost::posix_time::milliseconds(5000)); }
		virtual void onPause() { run_ = false; }
		virtual void onContinue() { run_ = true; }
		virtual void onShutdown() { run_ = false; }

	private:
		void startWorkers()
		{
			launchDebugger();

			if (run_)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(500));
			}
		}

		bool run_;
		int pref_;
	};
}

#endif