#include <functional>
#include <iostream>
#include <queue>
#include <map>
#include <array>
#include <memory>

class orderBook
{
public:
	orderBook()
	{
	}

	~orderBook()
	{
	}

	void add(const std::string & key, int val)
	{
		if(data_.find(key) == data_.end())
		{
			std::cout << "inserting new key: " << key << std::endl;

			// create the iterator
			std::map<std::string, int>::iterator it = data_.begin();
			data_.insert(it, std::make_pair(key, val));

			// insert the iterator
			p.push_back(&(it));
		}
		else
		{
			std::cout << "updating value: " << key << std::endl;
			data_[key] += val;
		}
	}

	void print()
	{
		std::cout << "summary of the map: " << std::endl;

		for (auto & i : data_)
			std::cout << i.first << ", " << i.second << std::endl;

		std::cout << "first element of the heap: " << std::endl;
		std::cout << p[0]->first << ", " << p[0]->second << std::endl;
	}

private:

	void resizeHeap(int sz);

	std::map<std::string, int> data_;

	std::vector<std::pair<const std::string, int>*> p;
};
 
int main() 
{
	orderBook my;

	my.add("MSFT", 100);
	my.add("MSFT", 50);
	my.add("AAPL", 100);
	//my.add("FB", 200);

	my.print();
 
	return 0;
}
