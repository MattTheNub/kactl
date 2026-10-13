/**
 * Author: Rishikesh
 * Date: 2026-10-12
 * Source: https://math.mit.edu/~goemans/18453S17/matroid-intersect-notes.pdf
 * Description: Returns a largest common independent set of two
 * matroids on elements $[0,n)$, in sorted order.
 * Each oracle must provide clear() (reset to the empty set),
 * add(x) (insert an element known to preserve independence),
 * and check(x) (test whether x can be added independently,
 * without changing the represented set). The oracles must have
 * separate state; their initial contents are ignored, and both
 * contain the returned set on exit.
 * Time: O(n(r+1)^2 T), where r is the answer size and T bounds
 * each oracle operation. Uses O(n(r+1)) extra space.
 * Status: stress-tested
 */
#pragma once

template<class M1, class M2>
vi matroidIntersection(M1& m1, M2& m2, int n) {
	vi in(n);
	for (;;) {
		vi cur, out;
		krep(x,0,n) (in[x] ? cur : out).push_back(x);
		auto build = [&](int skip) {
			m1.clear(); m2.clear();
			for (int x : cur) if (x != skip)
				m1.add(x), m2.add(x);
		};
		int s = n, t = n + 1;
		vector<vi> adj(n + 2);
		build(-1);
		for (int x : out) {
			if (m1.check(x)) adj[s].push_back(x);
			if (m2.check(x)) adj[x].push_back(t);
		}
		for (int y : cur) {
			build(y);
			for (int x : out) {
				if (m1.check(x)) adj[y].push_back(x);
				if (m2.check(x)) adj[x].push_back(y);
			}
		}
		vi par(n + 2, -1), q = {s};
		par[s] = s;
		// A shortest augmenting path preserves independence.
		krep(i,0,sz(q)) {
			if (par[t] != -1) break;
			for (int v : adj[q[i]]) if (par[v] == -1)
				par[v] = q[i], q.push_back(v);
		}
		if (par[t] == -1) {
			build(-1);
			return cur;
		}
		for (int x = par[t]; x != s; x = par[x])
			in[x] ^= 1;
	}
}
