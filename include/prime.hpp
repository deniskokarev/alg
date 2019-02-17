#ifndef __PRIME_HH__
#define __PRIME_HH__

#include <cinttypes>
#include <utility>

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


#endif // __PRIME_HH__
