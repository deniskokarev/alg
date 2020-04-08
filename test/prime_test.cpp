#include "prime.hpp"
#include "gtest/gtest.h"
#include <numeric>

#define dim(X)	(sizeof(X)/sizeof(X[0]))

template<class IT> bool vector_cmp(IT b1, IT e1, IT b2) {
	bool ans = true;
	while (b1<e1)
		ans &= (*b1++ == *b2++);
	return ans;
}

TEST(Prime, FactorsBasic) {
	uint64_t exp_ff[] = {2, 3, 5, 7};
	uint64_t prod = std::accumulate(exp_ff, exp_ff+dim(exp_ff), 1ULL, std::multiplies<uint64_t>());
	uint64_t ff[64];
	int ff_sz = prime_factors_all(prod, ff);
	EXPECT_EQ(std::accumulate(ff, ff+ff_sz, 1ULL, std::multiplies<uint64_t>()), prod);
	EXPECT_TRUE(vector_cmp(ff, ff+ff_sz, exp_ff));
}

TEST(Prime, FactorsLimit1) {
	// upto 2**63
	uint64_t exp_ff[] = {
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
	};
	uint64_t prod = std::accumulate(exp_ff, exp_ff+dim(exp_ff), 1ULL, std::multiplies<uint64_t>());
	uint64_t ff[64];
	int ff_sz = prime_factors_all(prod, ff);
	EXPECT_EQ(std::accumulate(ff, ff+ff_sz, 1ULL, std::multiplies<uint64_t>()), prod);
	EXPECT_TRUE(vector_cmp(ff, ff+ff_sz, exp_ff));
}

TEST(Prime, FactorsLimit2) {
	// upto 2**64-1
	uint64_t exp_ff[] = {3, 5, 17, 257, 641, 65537, 6700417};
	uint64_t prod = std::accumulate(exp_ff, exp_ff+dim(exp_ff), 1ULL, std::multiplies<uint64_t>());
	uint64_t ff[64];
	int ff_sz = prime_factors_all(prod, ff);
	EXPECT_EQ(std::accumulate(ff, ff+ff_sz, 1ULL, std::multiplies<uint64_t>()), prod);
	EXPECT_TRUE(vector_cmp(ff, ff+ff_sz, exp_ff));
}

template<class N> N ipow(N b, int p) {
	if (p) {
		N r = ipow(b, p/2);
		if (p&1)
			return r*r*b;
		else
			return r*r;
	} else {
		return 1;
	}
}

TEST(Prime, FactorsUniq) {
	uint64_t exp_ff[] = {
		2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3,
		5, 5, 5, 5, 5, 5, 5, 5,
		7, 7, 7, 7, 7, 7, 7, 7
	};
	uint64_t prod = std::accumulate(exp_ff, exp_ff+dim(exp_ff), 1ULL, std::multiplies<uint64_t>());
	std::pair<uint64_t,int> uqff[15];
	int uqff_sz = prime_factors_uniq(prod, uqff);
	uint64_t prod2 = std::accumulate(uqff, uqff+uqff_sz, 1ULL, [](uint64_t acc, const std::pair<uint64_t,int> &x){ return acc*ipow(x.first, x.second);});
	EXPECT_EQ(prod, prod2);
}

TEST(Prime, FactorsUniqLimit) {
	uint64_t ff[] = {
		2, 2, 3, 3, 5, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
	};
	std::pair<uint64_t,int> exp[] = {
		{2, 2}, {3, 2}, {5, 2}, {7, 1}, {11, 1}, {13, 1}, {17, 1}, {19, 1}, {23, 1}, {29, 1}, {31, 1}, {37, 1}, {41, 1}, {43, 1}, {47, 1}
	};
	uint64_t prod = std::accumulate(ff, ff+dim(ff), 1ULL, std::multiplies<uint64_t>());
	std::pair<uint64_t,int> uqff[15];
	int uqff_sz = prime_factors_uniq(prod, uqff);
	uint64_t prod2 = std::accumulate(uqff, uqff+uqff_sz, 1ULL, [](uint64_t acc, const std::pair<uint64_t,int> &x){ return acc*ipow(x.first, x.second);});
	EXPECT_EQ(prod, prod2);
	EXPECT_TRUE(vector_cmp(uqff, uqff+uqff_sz, exp));
}

TEST(Prime, SieveCheck3) {
	auto seave = prime_sieve(4);
	EXPECT_TRUE(!seave[0] && !seave[1] && seave[2] && seave[3]);
}

static const std::vector<int> primes_1000 {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
	79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157,
	163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
	241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331,
	337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
	431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509,
	521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
	617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709,
	719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
	823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919,
	929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997
};

TEST(Prime, SieveCheck1000) {
	int MX = primes_1000.back();
	auto seave = prime_sieve(MX+1);
	for (auto p:primes_1000)
		EXPECT_TRUE(seave[p]);
	for (int i=0; i<MX; i++) {
		auto fnd = std::lower_bound(primes_1000.begin(), primes_1000.end(), i);
		if (seave[i])
			EXPECT_TRUE(*fnd == i);
		else
			EXPECT_TRUE(*fnd > i);
	}
}
