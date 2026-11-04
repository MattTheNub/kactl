/**
 * Author: OpenAI
 * Date: 2026-11-04
 * License: CC0
 * Description: Builds the centroid tree of an undirected tree.
 *  par[v] is the centroid parent of v, or -1 at the root.
 * Time: O(N \log N), O(N) space.
 * Status: stress-tested
 */
#pragma once

struct Centroid {
	vector<vi> adj;
	vi par, siz, dead;
	Centroid(vector<vi> g)
		: adj(g), par(sz(g), -1), siz(sz(g)), dead(sz(g)) {
		if (sz(g)) build(0, -1);
	}
	int dfs(int v, int p) {
		siz[v] = 1;
		for (int u : adj[v]) if (u != p && !dead[u])
			siz[v] += dfs(u, v);
		return siz[v];
	}
	int centroid(int v, int p, int n) {
		for (int u : adj[v]) if (u != p && !dead[u])
			if (siz[u] > n/2) return centroid(u, v, n);
		return v;
	}
	void build(int v, int p) {
		int c = centroid(v, -1, dfs(v, -1));
		par[c] = p;
		dead[c] = 1;
		for (int u : adj[c]) if (!dead[u]) build(u, c);
	}
};
