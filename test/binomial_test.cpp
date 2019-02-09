#include "binomial.hpp"
#include "gtest/gtest.h"

TEST(Binomial, BinomialAll) {
	auto v = CHOOSE(100000000, 2);
	EXPECT_EQ(100000000ULL*(100000000-1)/2, v);
	v = CHOOSE(1, 1);
	EXPECT_EQ(1, v);
	v = CHOOSE(2, 1);
	EXPECT_EQ(2, v);
	v = CHOOSE(2, 2);
	EXPECT_EQ(1, v);
	v = CHOOSE(61, 60);
	EXPECT_EQ(v, 61);
}
