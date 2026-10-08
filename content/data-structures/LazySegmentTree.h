/**
 * Author: JDUREI LZSEG RAAAAAAAAAAAAAAAAAAAAAa
 * Date: 2016-10-08
 * License: CC0
 * Source: me
 * Description: Zero-indexed iterative lazy segment tree on inclusive intervals
 * [l, r]. Customize f, g, h, and the identities for the desired operations.
 * Time: O(\log N).
 * Usage: lzseg tree(n); tree.modify(i, value); tree.modify(l, r, update); auto result = tree.query(l, r);
 * Status: jdurie says it's probably correct but he's still not sure
 */
#pragma once

struct lzseg {
	typedef ll T;
	typedef ll U;
	T idT = 0;
	U idU = 0;
	ll n, lg = 0;
	vector<T> t;
	vector<U> d;
	lzseg(ll n) : n(n), t(2*n, idT), d(n, idU) {
		for (ll i = 1; i < n; i *= 2) ++lg;
	}
	// combining segtree nodes a and b
	T f(T a, T b) { return a + b; }
	// applying updates a and b (in that order)
	U g(U b, U a) { return a + b; }
	// applying update b to segtree node a
	T h(U b, T a) { return a + b; }
	void calc(ll p) { t[p] = h(d[p], f(t[p * 2], t[p * 2 + 1])); }
	void apply(ll p, U v) {
		t[p] = h(v, t[p]);
		if(p < n) d[p] = g(v, d[p]);
	}
	void push(ll p) {
		p += n;
		for (ll s = lg; s > 0; --s) {
			ll i = p >> s;
			if(i && d[i] != idU) {
				apply(i * 2, d[i]);
				apply(i * 2 + 1, d[i]);
				d[i] = idU;
			}
		}
	}
	void modify(ll p, T v) {
		push(p);
		t[p += n] = v;
		while(p > 1) calc(p /= 2);
	}
	void modify(ll l, ll r, U v) {
		push(l), push(r);
		bool cl = false, cr = false;
		for(l += n, r += n + 1; l < r; l /= 2, r /= 2) {
			if(cl) calc(l - 1);
			if(cr) calc(r);
			if(l & 1) apply(l++, v), cl = true;
			if(r & 1) apply(--r, v), cr = true;
		}
		for(--l; r; l /= 2, r /= 2) {
			if(cl) calc(l);
			if(cr) calc(r);
		}
	}
	T query(ll l, ll r) {
		push(l), push(r);
		T resl = idT, resr = idT;
		for(l += n, r += n + 1; l < r; l /= 2, r /= 2) {
			if(l & 1) resl = f(resl, t[l++]);
			if(r & 1) resr = f(t[--r], resr);
		}
		return f(resl, resr);
	}
};
