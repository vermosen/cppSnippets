#include <iostream>

#include "timer.h"

#ifdef __INTEL_COMPILER
#define EIGEN_USE_MKL_ALL
#define EIGEN_NO_DEBUG
#elif __GNUC__

#else
//#define EIGEN_USE_MKL_VML
#endif

#include <Eigen/Dense>

int main() {

	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
	Eigen::MatrixXf X = Eigen::MatrixXf::Random(2000, 2000);
	Eigen::MatrixXf A = X + X.transpose();

	TIMED_TEST(
		es.compute(A);
	)
	
	TIMED_TEST_MESSAGE();

	//std::cout
	//	<< "The eigenvalues of A are: "
	//	<< es.eigenvalues().transpose()
	//	<< std::endl;

	system("pause");
	return EXIT_SUCCESS;
}

// icc -O2 -DEIGEN_USE_MKL_VML			      6260760
// icc -O2 -avx2 -DEIGEN_USE_MKL_VML -DNDEBUG 4079870
// icc -O2 -avx2 -DEIGEN_USE_MKL_VML          4195861
// icc -O2 -avx2						      4349201
// icc -O2									  6479226
// msvc -O2									 12872299
// msvc -O2 -avx2							 11450529
// msvc -O2 -avx2 EIGEN_USE_MKL_VML			 11573637