/**
 * Author: Astra
 * Date: 2026-10-07
 * License: CC0
 * Description: Solves $F(a)=0$ modulo $x^n$, starting
 * from a constant root $a_0$. f(a, m) returns a pair
 * $(F(a), F'(a))$ modulo $x^m$; derivative is w.r.t. a.
 * Requires $F'(a_0)[0] \ne 0$, $n \ge 0$, coefficients
 * in [0, mod). Each step uses $a \gets a-F(a)/F'(a)$.
 * Time: O(n \log n) if f takes O(m \log m)
 * Status: stress-tested
 */
#pragma once

#include "PolyInv.h"

template<class F>
vl newton(ll a0, int n, F f) {
	if (!n) return {};
	vl a{a0};
	while (sz(a) < n) {
		int m = min(2 * sz(a), n);
		auto [b, c] = f(a, m);
		b.resize(m);
		b = conv(b, polyInv(c, m));
		a.resize(m);
		krep(i,0,m) a[i] = (a[i] - b[i] + mod) % mod;
	}
	return a;
}
