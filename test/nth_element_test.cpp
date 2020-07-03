#include "nth_element.hpp"
#include "gtest/gtest.h"

TEST(NthElement, NthElementSimple) {
	std::vector<int> data {1,2,3,4,5,6,7,8,9};
	std::random_shuffle(data.begin(), data.end());
	my_nth_element(data, 4);
	EXPECT_EQ(5, data[4]);
}

TEST(NthElement, NthElementManyPerm) {
	// median is always median in any randomly shuffled permutation
	for (int sz=1; sz<128; sz++) {
		std::vector<int> data(sz);
		for (int i=0; i<sz; i++)
			data[i] = i;
		std::random_shuffle(data.begin(), data.end());
		int mid = sz/2;
		my_nth_element(data, mid);
		EXPECT_EQ(mid, data[mid]);
	}
}

TEST(NthElement, NthElementVsNthElement) {
	// rendering random data sets with repetitions and comparing with standard nth_element
	for (int iter=0; iter<1024; iter++) {
		int sz = 1024;
		std::vector<int> data(sz);
		for (int i=0; i<sz; i++)
			data[i] = int64_t(std::rand())*sz/10/RAND_MAX;
		// run standard
		std::vector<int> data_std(data);
		std::nth_element(data_std.begin(), data_std.begin()+iter, data_std.end());
		// run ours
		my_nth_element(data, iter);
		// both must match
		EXPECT_EQ(data_std[iter], data[iter]);
	}
}
