/**
 * Author: Johan Sannemo, Simon Lindholm, Luke Videckis
 * Date: 2016-12-15
 * License: CC0
 * Source: https://github.com/kth-competitive-programming/kactl/pull/314
 * Description: Finds a minimum vertex cover in a bipartite graph.
 *  $g$ lists neighbors of the left partition. Requires an
 *  existing maximum matching $r$ from HopcroftKarp, where
 *  $r[v]$ is the left match of right vertex $v$, or $-1$.
 *  Returns two 0/1 vectors $cl, cr$, indexed within each side:
 *  $cl[u] = 1$ iff left vertex $u$ is in the cover, and
 *  $cr[v] = 1$ iff right vertex $v$ is in the cover.
 *  The size is the same as the size of a maximum matching, and
 *  the complement is a maximum independent set.
 * Usage: vi r(m, -1); hopcroftKarp(g, r);
 *  auto [cl, cr] = cover(g, r);
 * Time: O(V + E), given a maximum matching.
 * Status: stress-tested
 */
#pragma once

#include "HopcroftKarp.h"

pair<vi, vi> cover(vector<vi>& g, vi& r) {
	int n = sz(g), t = 0;
	vi cl(n), cr(sz(r)), q(n);
	for (int u : r) if (u != -1) cl[u] = 1;
	rep(i,0,n) if (!cl[i]) q[t++] = i;
	rep(i,0,t) for (int v : g[q[i]]) {
		cr[v] = 1;
		if (r[v] != -1 && cl[r[v]]) cl[q[t++] = r[v]] = 0;
	}
	return {cl, cr};
}
