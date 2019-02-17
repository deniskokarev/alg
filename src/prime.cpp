#include "prime.hpp"

/**
 * Naive prime number factorization in O(sqrt(N))
 * @param n - number that you want to factorize
 * @param p[] - preallocated recipient array where the of ordered
 *   prime numbers of n will be placed
 * 	p.length >= 64
 * each factor may occur with repetitions
 * @return number of populated primes in p[]
 */
int prime_factors_all(uint64_t n, uint64_t p[]) {
	int np = 0;
	for (uint64_t i = 2; i <= n / i; i++) {
		while (n % i == 0) {
			p[np++] = i;
			n /= i;
		}
	}
	if (n > 1)
		p[np++] = n;
	return np;
}

/**
 * Naive prime number factorization in O(sqrt(N))
 * @param n - number that you want to factorize
 * @param p[] - preallocated pairs array where the of ordered
 *   prime factors with their respective powers will be placed
 * 	p.length >= 15
 * each factor may occur with repetitions
 * @return number of populated primes in p[]
 */
int prime_factors_uniq(uint64_t n, std::pair<uint64_t,int> p[]) {
	int np = 0;
	for (uint64_t i = 2; i <= n / i; i++) {
		if (n % i == 0) {
			p[np] = std::make_pair(i, 1);
			n /= i;
			while (n % i == 0) {
				p[np].second++;
				n /= i;
			}
			np++;
		}
	}
	if (n > 1)
		p[np++] = std::make_pair(n, 1);
	return np;
}
