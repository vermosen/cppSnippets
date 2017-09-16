//============================================================================
// Name        : problem1.cpp
// Author      : jean-mathieu vermosen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>


template <typename T>
void swap(T & i, T & j)
{
    std::swap<T>(i, j);
}

template <>
void swap(int & i, int & j)
{
	i += j;
	j = i - j;
	i -= j;
}

void bubbleSort(int arr[], int n)
{
   int i, j;

   for (i = 0; i < n - 1; i++)
       for (j = 0; j < n - i - 1; j++)        // Last i elements are already in place
           if (arr[j] > arr[j + 1])
              swap(arr[j], arr[j + 1]);
}

int main()
{
	/*
	 * Bubble Sort is the simplest sorting algorithm
	 * that works by repeatedly swapping the adjacent
	 * elements if they are in wrong order.
         * Complexity is O(n^2)
	 */
	int arr[] = {4, 3, 40, 10, 5};
                
	int n = sizeof(arr)/ sizeof(arr[0]);
	bubbleSort(arr, n);

	std::cout << arr[0] << ", " << arr[1] << std::endl;
	return 0;
}
