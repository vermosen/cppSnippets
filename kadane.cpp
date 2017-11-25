//============================================================================
// Name        : kadane.cpp
// Author      : jean-mathieu vermosen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

void kadane(double arr[], int sz, int * begPtr, int * endPtr, double * sumPtr)
{
	int beg = 0; int end = 0;
	double max_here = arr[beg];
	double sum = arr[beg];

	for(int i = 1; i < sz; i++)
	{
		max_here += arr[i];
		if(arr[i] > max_here)
		{
			beg = i;
			max_here = arr[i];
		}

		if(max_here > sum)
		{
			sum = max_here;
			end = i;
		}
	}

	*begPtr = beg;
	*endPtr = end;
	*sumPtr = sum;
}

int main()
{
	/*
	 * The Kadane algorithm is used to find the sub-array
	 * with the biggest partial sum of elements
	 */
	double arr[] = {4.2, -1.2, -10, 10.25, -2.25, 20.2,  0.1};
	int beg = 0; int end = 0; double sum = 0;
	int * begPtr = &beg; int * endPtr = &end;
	double * sumPtr = &sum;
	int sz = sizeof(arr) / sizeof(arr[0]);

	kadane(arr, sz, begPtr, endPtr, sumPtr);

	std::cout
		<< "array starting at "
		<< *begPtr
		<< " and ending at "
		<< *endPtr
		<< " sums to "
		<< *sumPtr
		<< std::endl;

	return 0;
}
