#ifndef __PARTITION_HH__
#define __PARTITION_HH__

#include <vector>

struct Range {
	size_t begin, end;
};

// rearrangle [l..r] section of vector vv where vv[r] is considered a pivot element
// first we have elements less than pivot, then equal to pivot and then greader than pivot
// @return Range (b,e), where vv[l..b-1) < pivot, vv[b..e) == pivot and vv[e..r] > pivot
template<class T> Range partition3way(std::vector<T> &vv, size_t l, size_t r) {
	size_t mid = l;
	const T pivot = vv[r];
	while (mid <= r) {
		if (vv[mid] < pivot)
			std::swap(vv[l++], vv[mid++]);
		else if (pivot < vv[mid])
			std::swap(vv[mid], vv[r--]);
		else
			mid++;
	}
	return Range {l, mid};
}

#endif // __PARTITION_HH__
