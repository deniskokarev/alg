#ifndef __SEGTREE_HH__
#define __SEGTREE_HH__

#include <functional>
#include <vector>
#include <cmath>

/**
 * Simple bottom-up Segment tree on a vector with custom "fold" operation.
 * Supports setting a value at a position and "folding" values on a range
 * Typically, folding would be std::plus
 * O(2*n) space
 * set() - O(NlogN) time
 * fold() - O(NlogN) time
 * @author Denis Kokarev
 */
template<class ValueType=int, class FoldOp=std::plus<ValueType>> class BotUpSegTree {
	using value_type = ValueType;
	int sz;
	std::vector<value_type> tree; // where we keep all values, first sz elements are aggreagates
	/**
	 * If you touched values directly, you must perform rebuild()
	 */
	void rebuild() {
		for (int i=sz+sz-1; i>1; i-=2)
			tree[i>>1] = FoldOp()(tree[i-1], tree[i]);
	}
public:
	/**
	 * Create segment tree with given size and custom fold operator
	 * Upon creation all values will be zeros
	 * @param _sz - maximum size
	 * @param _fold  - custom fold operator - default std::plus
	 */
	BotUpSegTree(int sz):sz(sz),tree(sz+sz) {
	}

	/**
	 * Alternative constructor to initialize the values right away
	 * Works in O(n)
	 * @param list - list of given values
	 * @param _fold  - custom fold operator - default std::plus
	 */
	BotUpSegTree(const std::initializer_list<value_type> &list):BotUpSegTree(list.size()) {
		std::copy(list.begin(), list.end(), tree.begin()+sz);
		rebuild();
	}
	
	/**
	 * Perform online update in O(logN)
	 * @param pos
	 * @param v
	 */
	void set(int pos, const value_type &v) {
		int node = sz+pos;
		tree[node] = v;
		node >>= 1;
		if (((node<<1)|1) < sz+sz)
			tree[node] = FoldOp()(tree[node<<1], tree[(node<<1)|1]);
		else
			tree[node] = tree[node<<1];
		for (node=node>>1; node>0; node >>= 1)
			tree[node] = FoldOp()(tree[node<<1], tree[(node<<1)|1]);
	}

	/**
	 * Perform interval folding on open-ended [b, e) segment. Runs in O(logN)
	 * @param b - begin - first element inclusive
	 * @param e - end - element after last
	 * @return fold(tree[b], fold(tree[b+1], fold(tree[b+2], ... fold(tree[e-2], fold(tree[e-1])...)))
	 */
	value_type operator()(int b, int e) const {
		b += sz;
		e += sz;
		if (e-b > 1) {
			value_type vb = value_type();
			value_type ve = value_type();
			while (b < e) {
				if (b&1)
					vb = FoldOp()(vb, tree[b++]);
				if (e&1)
					ve = FoldOp()(tree[--e], ve);
				b >>= 1;
				e >>= 1;
			}
			return FoldOp()(vb, ve);
		} else if (e-b == 1) {
			return tree[b];
		} else {
			return value_type();
		}
	}
	/**
	 * Perform entire interval folding
	 * @return fold(tree[b], fold(tree[b+1], fold(tree[b+2], ... fold(tree[e-2], fold(tree[e-1])...)))
	 */
	value_type operator()() {
		return operator()(0, sz);
	}
};

/**
 * Segment tree to perform range increments on all values in the open interval [b, e)
 * Then you can gen any element in O(logN)
 * or recompute all elements in O(N)
 */
template<class ValueType=int> class TopDownSegTree {
	using value_type = ValueType;
	int sz;
	int l2;
	std::vector<value_type> tree; // where we keep all values (first sz elements are aggreagates)
	/**
	 * Propagate increments down to a specific element in O(logN)
	 */
	void propagate_inc(int pos) {
		for (int l=l2; l>0; l--) {
			int p = pos >> l;
			tree[p<<1] += tree[p];
			tree[(p<<1)|1] += tree[p];
			tree[p] = value_type();
		}
	}
	/**
	 * Propagate all increments to values in O(n)
	 */
	void propagate_inc() {
		for (int i=1; i < sz; i++) {
			tree[i<<1] += tree[i];
			tree[(i<<1)|1] += tree[i];
			tree[i] = value_type();
		}
	}
public:
	/**
	 * Create segment tree with given size
	 * Upon creation all values will be zeros
	 * @param sz - maximum size
	 */
	TopDownSegTree(int sz):sz(sz),l2(floor(log2(sz))),tree(sz+sz) {
	}

	TopDownSegTree(const std::initializer_list<value_type> &list):TopDownSegTree(list.size()) {
		std::copy(list.begin(), list.end(), tree.begin()+sz);
	}
	
	/**
	 * Perform "lazy" increment by v on all values in the open interval [b, e)
	 * Runs in O(logN)
	 */ 
	void inc(int b, int e, const value_type &v) {
		b += sz;
		e += sz;
		while (b < e) {
			if (b&1)
				tree[b++] += v;
			if (e&1)
				tree[--e] += v;
			b >>= 1;
			e >>= 1;
		}
	}

	/**
	 * Get the value at position pos in O(logN)
	 */
	const value_type &get(int pos) {
		int node = sz+pos;
		propagate_inc(node);
		return tree[node];
	}
	
	/**
	 * Or if want to recompute all values and iteratite them
	 * Needs O(N) to precompute
	 */
	typename std::vector<value_type>::const_iterator begin() {
		propagate_inc();
		return tree.cbegin() + sz;
	}
	
	typename std::vector<value_type>::const_iterator end() {
		return tree.cend();
	}
};

/**
 * Segment tree to perform "lazy" Increments on all values in the open interval [b, e)
 * Then you can still perofrm Fold over the range. FoldOp must be supporting 2 operations:
 *    fold two ValueTypes and fold ValueType with LazyType and level
 * LazyTypes must support += between them
 * All operations run in O(logN)
 */
template<class ValueType, class LazyType, class FoldOp> class LazySegTree {
	using value_type = ValueType;
	using lazy_type = LazyType;
	int sz;
	int l2;
	std::vector<value_type> tree; // where we keep all values (first sz elements are aggreagates)
	std::vector<lazy_type> lazy;

	void rebuild_pos(int p) {
		int level = 1;
		while (p > 1) {
			p >>= 1;
			int c1 = p << 1;
			int c2 = c1 + 1;
			tree[p] = FoldOp()(FoldOp()(tree[c1], tree[c2]), lazy[p], level);
			level++;
		}
	}

	/**
	 * Propagate increments down to a specific element in O(logN)
	 */
	void propagate_inc(int pos) {
		for (int l=l2; l>1; l--) {
			int p = pos >> l;
			int c1 = p << 1;
			int c2 = c1 + 1;
			lazy[c1] += lazy[p];
			lazy[c2] += lazy[p];
			tree[c1] = FoldOp()(tree[c1], lazy[p], l-1);
			tree[c2] = FoldOp()(tree[c2], lazy[p], l-1);
			lazy[p] = lazy_type();
		}
		int l = 1;
		int p = pos >> l;
		int c1 = p << 1;
		int c2 = c1 + 1;
		tree[c1] = FoldOp()(tree[c1], lazy[p], l-1);
		tree[c2] = FoldOp()(tree[c2], lazy[p], l-1);
		lazy[p] = lazy_type();
	}
public:
	/**
	 * Create lazy segment tree with given size
	 * Upon creation all values will be zeros
	 * @param sz - maximum size
	 */
	LazySegTree(int _sz):sz(_sz),l2(floor(log2(sz))),tree(sz+sz),lazy(sz) {
	}

	value_type &operator[](int p) {
		return tree[p+sz];
	}
	
	void rebuild() {
		for (int i=sz+sz-1; i>1; i-=2)
			tree[i>>1] = FoldOp()(tree[i-1], tree[i]);
	}

	/**
	 * Perform "lazy" increment by v on all values in the open interval [b, e)
	 * Runs in O(logN)
	 */ 
	void inc(int b, int e, const lazy_type &v) {
		b += sz;
		e += sz;
		int rb = b;
		int re = e;
		int level = 0;
		if (b < e) {
			if (b&1) {
				tree[b] = FoldOp()(tree[b], v, level);
				b++;
			}
			if (e&1) {
				--e;
				tree[e] = FoldOp()(tree[e], v, level);
			}
			b >>= 1;
			e >>= 1;
			level++;
		}
		while (b < e) {
			if (b&1) {
				tree[b] = FoldOp()(tree[b], v, level);
				lazy[b] += v;
				b++;
			}
			if (e&1) {
				--e;
				tree[e] = FoldOp()(tree[e], v, level);
				lazy[e] += v;
			}
			b >>= 1;
			e >>= 1;
			level++;
		}
		rebuild_pos(rb);
		rebuild_pos(re-1);
	}

	/**
	 * Compute and get the value at position pos in O(logN)
	 */
	const value_type &get(int pos) {
		int node = sz+pos;
		propagate_inc(node);
		return tree[node];
	}

	/**
	 * Perform interval folding on open-ended [b, e) segment. Runs in O(logN)
	 * Folding is left-associative
	 * @param b - begin - first element inclusive
	 * @param e - end - element after last
	 * @return fold(tree[b], fold(tree[b+1], fold(tree[b+2], ... fold(tree[e-2], fold(tree[e-1])...)))
	 */
	value_type operator()(int b, int e) {
		b += sz;
		e += sz;
		propagate_inc(b);
		if (e-b > 1) {	
			value_type vb = value_type();
			value_type ve = value_type();
			propagate_inc(e-1);
			while (b < e) {
				if (b&1)
					vb = FoldOp()(vb, tree[b++]);
				if (e&1)
					ve = FoldOp()(tree[--e], ve);
				b >>= 1;
				e >>= 1;
			}
			return FoldOp()(vb, ve);
		} else if (e-b == 1) {
			return tree[b];
		} else {
			return value_type();
		}
	}
};

#endif //__SEGTREE_HH__
