/**
 * Author: Johan Sannemo
 * License: CC0
 * Description: Compute indices of smallest set of intervals covering another interval.
 * Intervals should be [inclusive, exclusive). To support [inclusive, inclusive],
 * change (A) to add \texttt{|| R.empty()}. Returns empty set on failure (or if G is empty).
 * Time: O(N \log N)
 * Status: Tested on kattis:intervalcover
 */
#pragma once

template<class T>
vi cover(pair<T, T> G, vector<pair<T, T>> I) {
	vi S(sz(I)), R;
	iota(all(S), 0);
	sort(all(S), [&](int a, int b) { return I[a] < I[b]; });
	T cur = G.f;
	int at = 0;
	while (cur < G.s) { // (A)
		pair<T, int> mx = make_pair(cur, -1);
		while (at < sz(I) && I[S[at]].f <= cur) {
			mx = max(mx, make_pair(I[S[at]].s, S[at]));
			at++;
		}
		if (mx.s == -1) return {};
		cur = mx.f;
		R.push_back(mx.s);
	}
	return R;
}
