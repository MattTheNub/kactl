/**
 * Author: unknown
 * Date: 2026-10-18
 * License: CC0
 * Source: folklore
 * Description: Add lines $kx+m$ and query maximum values (minimum if queryMax=false).
 *  Slopes must be nondecreasing for max, nonincreasing for min; query points
 *  must be nondecreasing. Queries require at least one line.
 *  Coefficient negation and evaluation arithmetic must fit in ll,
 *  and cross products in signed \texttt{\_\_int128}.
 * Time: O(1) amortized per operation.
 * Status: stress-tested
 */
#pragma once
#include <cassert>

struct CHT {
	struct Line {
		ll k, m;
		ll eval(ll x) const { return k*x + m; }
	};
	deque<Line> q;
	ll last = LLONG_MIN;
	int sign;
	CHT(bool queryMax = true) : sign(queryMax ? 1 : -1) {}
	bool bad(Line a, Line b, Line c) {
		return ((__int128)a.m - b.m) * ((__int128)c.k - b.k)
			>= ((__int128)b.m - c.m) * ((__int128)b.k - a.k);
	}
	void add(ll k, ll m) {
		k *= sign; m *= sign;
		assert(q.empty() || q.back().k <= k);
		if (!q.empty() && q.back().k == k) {
			if (q.back().m >= m) return;
			q.pop_back();
		}
		Line l{k, m};
		while (sz(q) >= 2 && bad(q[sz(q)-2], q.back(), l))
			q.pop_back();
		q.push_back(l);
	}
	ll query(ll x) {
		assert(!q.empty() && x >= last);
		last = x;
		while (sz(q) >= 2 && q[0].eval(x) <= q[1].eval(x))
			q.pop_front();
		return sign * q[0].eval(x);
	}
};
