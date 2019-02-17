#include "mat.hpp"
#include "gtest/gtest.h"

TEST(Mat, Det) {
	const int rs = 3, cs = 3;
	const int values[rs][cs] = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	};
	Mat<int> mm(rs, cs);
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			mm[i][j] = values[i][j];
	int det = mm.det();
	EXPECT_EQ(det, 0);
}

TEST(Mat, MultipleLineatRegression) {
	/* after https://www.hackerrank.com/challenges/s10-multiple-linear-regression */
	/* read input */
	const int m = 2, n = 7;
	Mat<double> F(n, m+1);
	const double ff[n][m+1] = {
		{1, 0.18, 0.89},
		{1, 1.0, 0.26},
		{1, 0.92, 0.11},
		{1, 0.07, 0.37},
		{1, 0.85, 0.16},
		{1, 0.99, 0.41},
		{1, 0.87, 0.47}
	};
	for (int i=0; i<n; i++)
		for (int j=0; j<=m; j++)
			F[i][j] = ff[i][j];
	const double yy[n] = {
		109.85,
		155.72,
		137.66,
		76.17,
		139.75,
		162.6,
		151.77
	};
	Mat<double> Y(n, 1);
	for (int i=0; i<n; i++)
		Y[i][0] = yy[i];
	const double xx[4][m+1] = {
		{1, 0.49, 0.18},
		{1, 0.57, 0.83},
		{1, 0.56, 0.64},
		{1, 0.76, 0.18}
	};
	const int q = 4;
	Mat<double> X(q, m+1);
	for (int i=0; i<q; i++)
		for (int j=0; j<=m; j++)
			X[i][j] = xx[i][j];
	/* "train" minsquare multiline regression */
	Mat<double> FT = F.transpose();
	Mat<double> B = FT.mul(F).inv().mul(FT).mul(Y);
	/* "infer" prediction */
	Mat<double> R = X.mul(B);
	/* check */
	const double expect[q] = {
		105.22,
		142.68,
		132.94,
		129.71
	};
	for (int i=0; i<q; i++)
		EXPECT_NEAR(R[i][0], expect[i], 0.01);		
}
