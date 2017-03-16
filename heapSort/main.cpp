#include <iostream>
#include <vector>

void heapify(int arr[], int n, int i)
{
	int largest = i;  // Initialize largest as root
	int l = 2 * i + 1;  // left = 2*i + 1
	int r = 2 * i + 2;  // right = 2*i + 2

						// If left child is larger than root
	if (l < n && arr[l] > arr[largest])
		largest = l;

	// If right child is larger than largest so far
	if (r < n && arr[r] > arr[largest])
		largest = r;

	// If largest is not root
	if (largest != i)
	{
		swap(arr[i], arr[largest]);

		// Recursively heapify the affected sub-tree
		heapify(arr, n, largest);
	}
}

	void sort()
	{
	
	}

	void push_back(T t)
	{
		vec_.push_back(t);
	}

	void swap(int i, int j)
	{
		T temp = vec_[i];
		vec_[i] = vec_[j];
		vec_[j] = temp;
	}

	auto begin() { return vec_.begin(); }
	auto end() { return vec_.end(); }
private:
	std::vector<T> vec_;
};
 
int main() 
{
	heapSort(my);

	int arr[22] = { 110, 5, 10,3 ,22, 100, 1, 23, 44, 12, 15, 110, 12, 10,3 ,22, 99, 1, 23, 56, 12, 32 };
	size_t sz = (sizeof(arr) / sizeof(arr[0])) - 1;

	quicksort(0, sz, arr);

	print(sz, arr);

	char c; std::cin >> c;
	return 0;
}
