//============================================================================
// Name        : kadane.cpp
// Author      : jean-mathieu vermosen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
#include <random>

void sampleExponential(double * arr, int sz, double lambda)
{
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(0, 1);

	for (int i = 0; i < sz; i++)
	{
		double x = dist(e2);
		arr[i] = - std::log(1.0 - x) / lambda;
	}
}

template<unsigned n>
double moment(double * arr, int sz)
{
	double mean = moment<1>(arr, sz);

	double cumul = 0.0;

	for(int i = 0; i < sz; i++)
	{
		cumul += std::pow((arr[i] - mean), n);
	}

	return cumul / sz;
}

template<>
double moment<1>(double * arr, int sz)
{
	double cumul = 0.0;

	for(int i = 0; i < sz; i++)
	{
		cumul += arr[i];
	}

	return cumul / sz;
}

int main()
{
	/*
	 * compute a sample according to some exponential
	 * distribution
	 */
	int sz = 10000;
	double * arr = (double *)malloc(sizeof(double) * sz);
	double lambda = 0.4;

	sampleExponential(arr, sz, lambda);

	std::cout << "mean: " << moment<1>(arr, sz) << std::endl;
	std::cout << "stddev: " << sqrt(moment<2>(arr, sz)) << std::endl;
	return 0;
}
