#include "segtree.hpp"
#include "gtest/gtest.h"
#include <numeric>

TEST(SegTree, Sum0) {
	SegTree<int> sum({1,2,3});
	EXPECT_EQ(sum(), 6);
	sum.set(0, 2);
	sum.set(1, 3);
	sum.set(2, 4);
	EXPECT_EQ(sum(), 9);
}

TEST(SegTree, Sum) {
	for (int sz=1; sz<128; sz++) {
		SegTree<int> sum(sz);
		for (int i=0; i<sz; i++)
			sum.set(i, i);
		for (int w=1; w<sz; w++) {
			for (int i=0; i<sz-w; i++) {
				int arsum = (i+w)*(i+w-1)/2 - i*(i-1)/2;
				EXPECT_EQ(sum(i, i+w), arsum);
			}
		}
	}
}

TEST(SegTree, Max) {
	for (int sz=1; sz<128; sz++) {
		SegTree<int> max(sz, [](int a, int b){return (a>b)?a:b;});
		for (int i=0; i<sz; i++)
			max.set(i, i);
		for (int w=1; w<sz; w++) {
			for (int i=0; i<sz-w; i++) {
				int mx = i+w-1;
				EXPECT_EQ(max(i, i+w), mx);
			}
		}
	}
}

TEST(AddSegTree, Add1) {
	AddSegTree<int> sum({1,2,3});
	sum.add_each(0, 3, 1);
	EXPECT_EQ(std::accumulate(sum.begin(), sum.end(), 0), 9);
}

TEST(AddSegTree, UpIncStat) {
	for (int sz=1; sz<128; sz++) {
		AddSegTree<int> sum(sz);
		for (int i=0; i<sz; i++)
			sum.add_each(i, sz, 1);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = i+1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), sum.begin()), true);
	}
}

TEST(AddSegTree, UpIncDyn) {
	for (int sz=1; sz<128; sz++) {
		AddSegTree<int> sum(sz);
		for (int i=0; i<sz; i++)
			sum.add_each(i, sz, 1);
		std::vector<int> ans(sz);
		for (int i=0; i<sz; i++)
			ans[i] = sum.get(i);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = i+1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), ans.begin()), true);
	}
}

TEST(AddSegTree, DownIncStat) {
	for (int sz=1; sz<128; sz++) {
		AddSegTree<int> sum(sz);
		for (int i=1; i<sz; i++)
			sum.add_each(0, i, 1);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = sz-i-1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), sum.begin()), true);
	}
}

TEST(AddSegTree, DownIncDyn) {
	for (int sz=1; sz<128; sz++) {
		AddSegTree<int> sum(sz);
		for (int i=1; i<sz; i++)
			sum.add_each(0, i, 1);
		std::vector<int> ans(sz);
		for (int i=0; i<sz; i++)
			ans[i] = sum.get(i);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = sz-i-1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), ans.begin()), true);
	}
}

TEST(AddSegTree, UpIncHalfSum) {
	for (int sz=1; sz<128; sz++) {
		AddSegTree<int> sum(sz);
		for (int i=0; i<=sz/2; i++)
			sum.add_each(i, sz-i, 1);
		int ans = 0;
		for (int i=0; i<sz; i++)
			ans += sum.get(i);
		int s1 = sz/2;
		int s2 = sz-s1;
		int arsum = s1*(s1+1)/2 + s2*(s2+1)/2;
		EXPECT_EQ(ans, arsum);
	}
}

TEST(AddSegTree, DownIncHalfSum) {
	for (int sz=1; sz<128; sz++) {
		AddSegTree<int> sum(sz);
		for (int i=sz/2; i>=0; i--)
			sum.add_each(i, sz-i, 1);
		int ans = 0;
		for (int i=sz-1; i>=0; i--)
			ans += sum.get(i);
		int s1 = sz/2;
		int s2 = sz-s1;
		int arsum = s1*(s1+1)/2 + s2*(s2+1)/2;
		EXPECT_EQ(ans, arsum);
	}
}

// need to init our int with 1
// inc is mul
struct M {
	int m;
	M(int m):m(m){}
	M():M(1){}
	void operator+=(const M &other) {
		m *= other.m;
	}
};

struct FoldSumMul {
	int64_t operator()(int64_t sm, M mul, int lvl) const {
		return sm * mul.m;
	}
	int64_t operator()(int64_t sm_l, int64_t sm_r) const {
		return sm_l + sm_r;
	}
};

TEST(LazySegTree, TestBasicSum) {
	const int sz = 8;
	LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
	for (int i=0; i<sz; i++)
		lzt[i] = i+1;
	lzt.rebuild();
	EXPECT_EQ(lzt(0, sz), sz*(sz+1)/2);
}

TEST(LazySegTree, BasicMul16) {
	const int sz = 16;
	LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
	for (int i=0; i<sz; i++)
		lzt[i] = 1;
	lzt.rebuild();
	lzt.inc(0, sz, 2);
	EXPECT_EQ(lzt(0, sz), 2*sz);
}

TEST(LazySegTree, BasicMul7) {
	const int sz = 7;
	LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
	for (int i=0; i<sz; i++)
		lzt[i] = 1;
	lzt.rebuild();
	lzt.inc(0, sz, M(2));
	EXPECT_EQ(lzt(0, sz), 2*sz);
}

TEST(LazySegTree, MulAll) {
	for (int sz=1; sz<128; sz++) {
		LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
		for (int i=0; i<sz; i++)
			lzt[i] = 3;
		lzt.rebuild();
		lzt.inc(0, sz, M(2));
		EXPECT_EQ(lzt(0, sz), 2*3*sz);
	}
}

TEST(LazySegTree, MulHalfs) {
	for (int sz=1; sz<128; sz++) {
		LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
		for (int i=0; i<sz; i++)
			lzt[i] = 3;
		lzt.rebuild();
		int h = sz/2;
		lzt.inc(0, h, M(2));
		lzt.inc(h, sz, M(2));
		EXPECT_EQ(lzt(0, sz), 2*3*sz);
	}
}

TEST(LazySegTree, MulLeft) {
	for (int sz=1; sz<128; sz++) {
		LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
		for (int i=0; i<sz; i++)
			lzt[i] = 3;
		lzt.rebuild();
		int h = sz/2;
		lzt.inc(0, h, M(2));
		EXPECT_EQ(lzt(0, sz), 2*3*h + 3*(sz-h));
	}
}
