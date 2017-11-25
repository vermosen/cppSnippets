#pragma once

#include <boost/noncopyable.hpp>

template <class T>
class singleton : private boost::noncopyable
{
public:
	//! access to the unique instance
	static T& instance();
protected:
	singleton() {}
};

template <class T>
T& singleton<T>::instance() 
{
	static boost::shared_ptr<T> instance_;

	if (!instance_)
		instance_ = boost::shared_ptr<T>(new T);
	return *instance_;
}