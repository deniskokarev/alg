#ifndef __PRIME_HH__
#define __PRIME_HH__

#include <cinttypes>
#include <utility>
#include <vector>

/**
 * Naive prime number factorization in O(sqrt(N))
 * @param n - number that you want to factorize
 * @param p[] - preallocated recipient array where the of ordered
 *   prime numbers of n will be placed
 * 	p.length >= 64
 * each factor may occur with repetitions
 * @return number of populated primes in p[]
 */
int prime_factors_all(uint64_t n, uint64_t p[]);

/**
 * Naive prime number factorization in O(sqrt(N))
 * @param n - number that you want to factorize
 * @param p[] - preallocated pairs array where the of ordered
 *   prime factors with their respective powers will be placed
 * 	p.length >= 15
 * each factor may occur with repetitions
 * @return number of populated primes in p[]
 */
int prime_factors_uniq(uint64_t n, std::pair<uint64_t,int> p[]);

/**
 * Render [0..n) vector of non-prime=false and prime=true indicators
 * for first n numbers
 */
std::vector<bool> prime_sieve(int n);

#endif // __PRIME_HH__
