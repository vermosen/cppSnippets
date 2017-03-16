#include <iostream>
#include <vector>

void print(int n, int * a)
{
	int i = 0; do  
	{
		std::cout << a[i] << ",";
	} 
	while (++i < n);
}

void swap(int i, int j, int * a) 
{
	if (i != j)
	{
		int temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}
}

void quicksort(int left, int right, int * arr)
{
	std::cout << "QS:" << left << "," << right << "\n";
	int mid = left + (right - left) / 2;

	int pivot = arr[mid];
	int i = left; int j = right;
	
	while (left < j || i < right)
	{
		while (arr[i] < pivot) i++;
		while (arr[j] > pivot) j--;

		if (i <= j)
		{
			swap(i++, j--, arr);
		}
		else 
		{
			if (left < j)
				quicksort(left, j, arr);
			if (i < right)
				quicksort(i, right, arr);
			return;
		}
	}
}

int main()
{
	int arr[22] = { 110, 5, 10,3 ,22, 100, 1, 23, 44, 12, 15, 110, 12, 10,3 ,22, 99, 1, 23, 56, 12, 32 };
	size_t sz = (sizeof(arr) / sizeof(arr[0])) - 1;

	quicksort(0, sz, arr);

	print(sz, arr);

	char c; std::cin >> c;
	return 0;
}
