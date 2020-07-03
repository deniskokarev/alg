#ifndef __NTH_ELEMENT_HH__
#define __NTH_ELEMENT_HH__

#include <vector>
#include "partition.hpp"

// compute k-th order statistics
template<class T> void my_nth_element(std::vector<T> &vv, size_t k) {
	size_t l=0, r=vv.size()-1;
	Range range = partition3way(vv, l, r);
	while (!(range.begin<=k && k<range.end)) {
		if (k < range.begin)
			r = range.begin-1;
		else
			l = range.end;
		range = partition3way(vv, l, r);
	}
}

#endif // __NTH_ELEMENT_HH__
