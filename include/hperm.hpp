#ifndef __HPERM_HH__
#define __HPERM_HH__

#include <algorithm>

/**
 * Efficient in-place Heap's permutation algorithm via interchanges
 * beg->end container will be shuffled (end-beg)! times
 * f(beg, end) will be called for every permutation
 */

namespace heaps_perm_r {
	template<typename I, typename F> void heaps_perm_r(const I &beg, const I &end, const I &last, F &&f) {
		if (end-beg == 1) {
			f(beg, last);
		} else if (end-beg > 1) {
			for (I i=beg; i!=(end-1); ++i) {
				heaps_perm_r(beg, end-1, last, f);
				if ((end-beg)%2) {
					std::swap(*beg, *(end-1));
				} else {
					std::swap(*i, *(end-1));
				}
			}
			heaps_perm_r(beg, end-1, last, f);
		}
	}
};

template<typename I, typename F> void heaps_perm(const I &beg, const I &end, F &&f) {
	heaps_perm_r::heaps_perm_r(beg, end, end, f);
}

#endif // __HPERM_HH__
