/**
 * Author: Lucian Bicsi
 * Date: 2017-10-31
 * License: CC0
 * Source: folklore
 * Description: Zero-indexed max-tree. Bounds are inclusive to the left and exclusive to the right.
 * Can be changed by modifying T, f and unit.
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

namespace seg {
	using T = ll;
	T id=0;
	T f(T a, T b) {return a+b;}
	T t[2 * NN];
	ll n=NN;  // array size
	void modify(ll p, T value) {  // set value at position p
		for (p+=n, t[p] = value; p /= 2;) t[p] = f(t[2*p], t[2*p+1]);
	}
	T query(ll l, ll r) { // fold f on interval [l, r)
		T resl=id, resr=id;
		for (l += n, r += n; l < r; l /= 2, r /= 2) {
			if (l&1) resl = f(resl, t[l++]);
			if (r&1) resr = f(t[--r], resr);
		}
		return f(resl, resr);
	}
}
