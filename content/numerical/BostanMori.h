/**
 * Author: Astra
 * Date: 2026-10-07
 * License: CC0
 * Description: Returns $[x^k]P(x)/Q(x)$. Requires
 * $Q[0] \ne 0$, $k \ge 0$, coefficients in [0, mod).
 * For a recurrence with initial terms $S$ and coefficients
 * $c$, use $Q = \{1,-c_0,-c_1,\ldots\}$ and
 * $P = S Q \bmod x^{|S|}$ (normalize coefficients).
 * Time: O(d \log d \log(k+1)), $d = \max(|P|, |Q|)$
 * Status: stress-tested
 */
#pragma once

#include "NumberTheoreticTransform.h"

ll bostanMori(vl p, vl q, ll k) {
	for (; k; k /= 2) {
		vl r = q;
		for (int i = 1; i < sz(r); i += 2)
			r[i] = (mod - r[i]) % mod;
		p = conv(p, r);
		q = conv(q, r);
		int n = 0;
		for (int i = int(k & 1); i < sz(p); i += 2)
			p[n++] = p[i];
		p.resize(n);
		n = 0;
		for (int i = 0; i < sz(q); i += 2) q[n++] = q[i];
		q.resize(n);
	}
	return p.empty() ? 0 : p[0] * modpow(q[0], mod-2) % mod;
}
