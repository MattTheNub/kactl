/**
 * Author: Astra
 * Date: 2026-10-07
 * License: CC0
 * Description: Returns $1/a$ modulo $x^n$. Requires
 * $a[0] \ne 0$, $n \ge 0$, coefficients in [0, mod).
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "NumberTheoreticTransform.h"

vl polyInv(const vl& a, int n) {
	if (!n) return {};
	vl b{modpow(a[0], mod - 2)};
	while (sz(b) < n) {
		int m = min(2 * sz(b), n);
		vl c(a.begin(), a.begin() + min(sz(a), m));
		c = conv(c, b);
		c.resize(m);
		for (ll& x : c) x = (mod - x) % mod;
		c[0] = (c[0] + 2) % mod;
		b = conv(b, c);
		b.resize(m);
	}
	return b;
}
