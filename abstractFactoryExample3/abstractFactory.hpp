#ifndef abstract_factory_hpp
#define abstract_factory_hpp

#include <map>
#include <boost/shared_ptr.hpp>

// an abstract factory class 
// with self management functionalities
template <typename C, typename K, typename Args>
class factory
{

protected:
	typedef K key_type;
	typedef std::map<key_type, C*(*)(Args)> map_type;

public:
	static boost::shared_ptr<C> createInstance(const K & key, Args args)
	{
		typename map_type::iterator it = getMap()->find(key);
		
		if (it == getMap()->end())
			throw std::runtime_error("unknown key found !");
		else
			return boost::shared_ptr<C>(it->second(args));
	}

protected:
	virtual ~factory()
	{
		if (map_)
		{
			map_->clear();
		}
	}
	static map_type * getMap()
	{
		if (!map_) { map_ = new map_type; }
		return map_;
	}

	static map_type * map_;

};

// create function
template <typename C, typename T, typename Args> C * create(Args args) { return new T(args); }

//registration struct
template <typename C, typename K, typename T, typename Args>
struct registerType : factory<C, K, Args>
{
	registerType(const K & key)
	{
		factory<C, K, Args>::getMap()->insert(std::make_pair(key, &create<C, T, Args>));
	}
};

#endif
