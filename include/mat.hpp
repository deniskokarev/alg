#ifndef __MAT_HH__
#define __MAT_HH__

/**
 * Naive Matrix arithmetic
 * @author Denis Kokarev
 */
#include <vector>
#include <cassert>
#include "hperm.hpp"

template<typename N> struct Mat {
	using VEC = std::vector<N>;
	int rows, cols;
	VEC vv;
	Mat(int _r, int _c):rows(_r),cols(_c),vv(rows*cols){}
	Mat(int _r, int _c, const VEC &_vv):rows(_r),cols(_c),vv(_vv){}
	Mat(const Mat &m):rows(m.rows),cols(m.cols),vv(m.vv){}
	Mat(Mat &&m):rows(m.rows),cols(m.cols),vv(std::move(m.vv)){}
	typename VEC::iterator operator[](int r) {
		return vv.begin()+r*cols;
	}
	typename VEC::const_iterator operator[](int r) const {
		return vv.cbegin()+r*cols;
	}
	N det() const {
		assert(rows == cols);
		const Mat &m = *this;
		int pp[rows];
		for (int i=0; i<rows; i++)
			pp[i] = i;
		N sum = 0;
		N po = +1;
		heaps_perm(&pp[0], &pp[cols], [&po, &sum, &m](const int *b, const int *e) {
				N s = 1;
				for (int ri=0; ri<m.rows; ri++)
					s *= m[ri][b[ri]];
				sum += s*po;
				po *= -1;
		});
		return sum;
	}
	Mat mat_minor(int r, int c) const {
		const Mat &me = *this;
		Mat res(rows-1, cols-1);
		for (int di=0,si=0; si<rows; si++) {
			if (si == r)
				continue;
			for (int dj=0,sj=0; sj<cols; sj++) {
				if (sj == c)
					continue;
				res[di][dj] = me[si][sj];
				dj++;
			}
			di++;
		}
		return res;
	}
	Mat adj() const {
		assert(rows == cols);
		if (rows < 2) {
			Mat a(1, 1);
			a[0][0] = 1;
			return a;
		} else {
			Mat a(rows, cols);
			for (int r=0; r<rows; r++) {
				int sign = ((r&1) == 0) ? 1:-1;
				for (int c=0; c<cols; c++) {
					Mat mr = mat_minor(r, c);
					a[c][r] = mr.det()*sign;
					sign *= -1;
				}
			}
			return a;
		}
	}
	Mat mul(const Mat &b) const {
		const Mat &a = *this;
		assert(a.cols == b.rows);
		Mat res(a.rows, b.cols);
		for (int r=0; r<a.rows; r++) {
			for (int c=0; c<b.cols; c++) {
				N s = 0;
				for (int j=0; j<a.cols; j++)
					s += a[r][j]*b[j][c];
				res[r][c] = s;
			}
		}
		return res;
	}
	// element-by-element operations
	void operator*=(N n) {
		for (auto &v:vv)
			v *= n;
	}
	void operator-=(const Mat &b) {
		assert(vv.size() == b.vv.size());
		for (int i=0; i<vv.size(); i++)
			vv[i] -= b.vv[i];
	}
	void operator+=(const Mat &b) {
		assert(vv.size() == b.vv.size());
		for (int i=0; i<vv.size(); i++)
			vv[i] += b.vv[i];
	}
	void operator=(const Mat<N> &b) {
		assert(vv.size() == b.vv.size());
		copy(b.vv.begin(), b.vv.end(), vv.begin());
	}
	Mat<N> operator-(const Mat<N> &b) const {
		assert(vv.size() == b.vv.size());
		Mat<N> res(*this);
		res -= b;
		return res;
	}
	Mat<N> operator+(const Mat<N> &b) const {
		assert(vv.size() == b.vv.size());
		Mat<N> res(*this);
		res += b;
		return res;
	}
	bool operator==(const Mat<N> &b) const {
		if (rows == b.rows && cols == b.cols) {
			for (int i=0; i<rows*cols; i++)
				if (vv[i] != b.vv[i])
					return false;
			return true;
		} else {
			return false;
		}
	}
	// for 1d vectors
	N length_squared() const {
		N s = 0;
		for (auto &v:vv)
			s += v*v;
		return s;
	}
	// NB: 1/det won't work on integral matrixes
	Mat inv() const {
		N d = det();
		assert(d != 0 && "if det == 0, we cannot find inverse");
		Mat res = adj();
		res *= 1/d;
		return res;
	}
	Mat transpose() const {
		const Mat &me = *this;
		Mat a(cols, rows);
		for (int i=0; i<rows; i++)
			for (int j=0; j<cols; j++)
				a[j][i] = me[i][j];
		return a;
	}
};

#endif // __MAT_HH__
