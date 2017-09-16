#pragma once
#ifndef WORKER_IMPL_HPP_
#define WORKER_IMPL_HPP_

#include <memory>

#include <boost/thread.hpp>

#include "threadUtil.hpp"

template <typename W>
class workerBase
{
protected:
	workerBase(const std::string & name)
		: name_(name) {}
	virtual ~workerBase() { /* interrupt ?*/ };

public:
	void start()
	{
		if (!t_)
		{
			t_ = std::move(std::unique_ptr<boost::thread>(new boost::thread(
				[&]() { static_cast<W*>(this)->work(); })));
			setThreadName(t_->get_id(), name_);
		}
	}

	void join()
	{
		if (t_) t_->join();
	}

private:
	std::unique_ptr<boost::thread> t_;
	std::string name_;
};

#endif