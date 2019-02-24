#include "segtree.hpp"
#include "gtest/gtest.h"
#include <numeric>
#include <random>

TEST(BotUpSegTree, Sum0) {
	BotUpSegTree<> sum({1,2,3});
	EXPECT_EQ(sum(), 6);
	sum.set(0, 2);
	sum.set(1, 3);
	sum.set(2, 4);
	EXPECT_EQ(sum(), 9);
}

TEST(BotUpSegTree, Sum) {
	for (int sz=1; sz<128; sz++) {
		BotUpSegTree<> sum(sz);
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

// helper class to perform max() on +
template<class N>struct Mx {
	using value_type = N;
	N i;
	Mx(N i):i(i){}
	Mx():Mx(0){}
	Mx operator+(const Mx &o) const {
		Mx res(*this);
		res.i = std::max(i, o.i);
		return res;
	}
	operator value_type() const {
		return i;
	}
};

TEST(BotUpSegTree, Max) {
	for (int sz=1; sz<128; sz++) {
		BotUpSegTree<Mx<int>> max(sz);
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

TEST(TopDownSegTree, TopDown1) {
	TopDownSegTree<int> sum({1,2,3});
	sum.inc(0, 3, 1);
	EXPECT_EQ(std::accumulate(sum.begin(), sum.end(), 0), 9);
}

TEST(TopDownSegTree, UpIncStat) {
	for (int sz=1; sz<128; sz++) {
		TopDownSegTree<int> sum(sz);
		for (int i=0; i<sz; i++)
			sum.inc(i, sz, 1);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = i+1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), sum.begin()), true);
	}
}

TEST(TopDownSegTree, UpIncDyn) {
	for (int sz=1; sz<128; sz++) {
		TopDownSegTree<int> sum(sz);
		for (int i=0; i<sz; i++)
			sum.inc(i, sz, 1);
		std::vector<int> ans(sz);
		for (int i=0; i<sz; i++)
			ans[i] = sum.get(i);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = i+1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), ans.begin()), true);
	}
}

TEST(TopDownSegTree, DownIncStat) {
	for (int sz=1; sz<128; sz++) {
		TopDownSegTree<int> sum(sz);
		for (int i=1; i<sz; i++)
			sum.inc(0, i, 1);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = sz-i-1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), sum.begin()), true);
	}
}

TEST(TopDownSegTree, DownIncDyn) {
	for (int sz=1; sz<128; sz++) {
		TopDownSegTree<int> sum(sz);
		for (int i=1; i<sz; i++)
			sum.inc(0, i, 1);
		std::vector<int> ans(sz);
		for (int i=0; i<sz; i++)
			ans[i] = sum.get(i);
		std::vector<int> res(sz);
		for (int i=0; i<sz; i++)
			res[i] = sz-i-1;
		EXPECT_EQ(std::equal(res.begin(), res.end(), ans.begin()), true);
	}
}

TEST(TopDownSegTree, UpIncHalfSum) {
	for (int sz=1; sz<128; sz++) {
		TopDownSegTree<int> sum(sz);
		for (int i=0; i<=sz/2; i++)
			sum.inc(i, sz-i, 1);
		int ans = 0;
		for (int i=0; i<sz; i++)
			ans += sum.get(i);
		int s1 = sz/2;
		int s2 = sz-s1;
		int arsum = s1*(s1+1)/2 + s2*(s2+1)/2;
		EXPECT_EQ(ans, arsum);
	}
}

TEST(TopDownSegTree, DownIncHalfSum) {
	for (int sz=1; sz<128; sz++) {
		TopDownSegTree<int> sum(sz);
		for (int i=sz/2; i>=0; i--)
			sum.inc(i, sz-i, 1);
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

TEST(LazySegTree, MulRand16) {
	int sz = 16;
	std::mt19937 rnd(1);
	std::vector<int64_t> vv(sz);
	LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
	for (int i=0; i<sz; i++)
		vv[i] = lzt[i] = 1;
	lzt.rebuild();
	EXPECT_EQ(lzt(0, sz), std::accumulate(vv.begin(), vv.end(), 0LL));
	std::vector<M> mul({2, 3, 5, 7, 11, 13});
	for (int i=0; i<10; i++) {
		int f = rnd() % sz;
		int t = std::min(sz, f + int(rnd() % sz));
		int mi = rnd() % mul.size();
		lzt.inc(f, t, mul[mi]);
		for (int j=f; j<t; j++)
			vv[j] *= mul[mi].m;
		int64_t asum = std::accumulate(vv.begin(), vv.end(), 0LL);
		EXPECT_EQ(lzt(0, sz), asum);
	}
}

TEST(LazySegTree, MulRandRand) {
	std::mt19937 rnd(1);
	for (int sz=1; sz<128; sz++) {
		std::vector<int64_t> vv(sz);
		LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
		for (int i=0; i<sz; i++)
			vv[i] = lzt[i] = 1;
		lzt.rebuild();
		EXPECT_EQ(lzt(0, sz), std::accumulate(vv.begin(), vv.end(), 0LL));
		std::vector<M> mul({2, 3, 5, 7, 11, 13});
		for (int i=0; i<10; i++) {
			int f = rnd() % sz;
			int t = std::min(sz, f + int(rnd() % sz));
			int mi = rnd() % mul.size();
			lzt.inc(f, t, mul[mi]);
			for (int j=f; j<t; j++)
				vv[j] *= mul[mi].m;
			f = rnd() % sz;
			t = std::min(sz, f + int(rnd() % sz));
			int64_t asum = std::accumulate(vv.begin()+f, vv.begin()+t, 0LL);
			EXPECT_EQ(lzt(f, t), asum);
		}
	}
}

TEST(LazySegTree, MulRandRandAndSet) {
	std::mt19937 rnd(1);
	for (int sz=1; sz<128; sz++) {
		std::vector<int64_t> vv(sz);
		LazySegTree<int64_t, M, FoldSumMul> lzt(sz);
		for (int i=0; i<sz; i++)
			vv[i] = lzt[i] = 1;
		lzt.rebuild();
		EXPECT_EQ(lzt(0, sz), std::accumulate(vv.begin(), vv.end(), 0LL));
		std::vector<M> mul({2, 3, 5, 7, 11, 13});
		for (int i=0; i<10; i++) {
			int f = rnd() % sz;
			int t = std::min(sz, f + int(rnd() % sz));
			int mi = rnd() % mul.size();
			lzt.inc(f, t, mul[mi]);
			for (int j=f; j<t; j++)
				vv[j] *= mul[mi].m;
			int64_t asum = std::accumulate(vv.begin(), vv.end(), 0LL);
			EXPECT_EQ(lzt(0, sz), asum);
			int pos = rnd() % sz;
			int64_t v = mul[rnd()%mul.size()].m;
			lzt.set(pos, v);
			vv[pos] = v;
			asum = std::accumulate(vv.begin(), vv.end(), 0LL);
			EXPECT_EQ(lzt(0, sz), asum);
		}
	}
}
