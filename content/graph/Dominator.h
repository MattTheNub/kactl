/**
 * Author: Unknown
 * Date: 2026-10-27
 * Source: User-provided implementation of Lengauer-Tarjan
 * Description: Computes the dominator tree of a directed
 * graph. A vertex u dominates v if every path from root to v
 * passes through u. Vertices are numbered from 0 to n-1.
 * After calc, ans contains the dominator tree's children;
 * parent(v) is v's immediate dominator, or -1 for the root
 * and unreachable vertices. Add all edges before calling
 * calc, and call calc exactly once with a valid root.
 * Usage: Dominator d(n); d.ae(a, b); d.calc(root);
 * Time: O((V + E) \log V)
 * Status: Stress-tested
 */
#pragma once

struct Dominator {
	int co = 0;
	vector<vi> adj, ans, radj, child, bucket;
	vi label, rlabel, sdom, dom, par, best;
	Dominator(int n) : adj(n), ans(n), radj(n+1),
		child(n+1), bucket(n+1), label(n), rlabel(n+1, -1),
		sdom(n+1), dom(n+1), par(n+1), best(n+1) {}
	void ae(int a, int b) { adj[a].push_back(b); }
	int get(int x) {
		if (par[x] != x) {
			int t = get(par[x]);
			par[x] = par[par[x]];
			if (sdom[t] < sdom[best[x]]) best[x] = t;
		}
		return best[x];
	}
	void dfs(int x) {
		label[x] = ++co;
		rlabel[co] = x;
		sdom[co] = par[co] = best[co] = co;
		for (int y : adj[x]) {
			if (!label[y]) {
				dfs(y);
				child[label[x]].push_back(label[y]);
			}
			radj[label[y]].push_back(label[x]);
		}
	}
	void calc(int root) {
		dfs(root);
		for (int i = co; i >= 1; --i) {
			for (int j : radj[i])
				sdom[i] = min(sdom[i], sdom[get(j)]);
			if (i > 1) bucket[sdom[i]].push_back(i);
			for (int j : bucket[i]) {
				int k = get(j);
				dom[j] = sdom[j] == sdom[k] ? sdom[j] : k;
			}
			for (int j : child[i]) par[j] = i;
		}
		rep(i,2,co+1) {
			if (dom[i] != sdom[i]) dom[i] = dom[dom[i]];
			ans[rlabel[dom[i]]].push_back(rlabel[i]);
		}
	}
	int parent(int x) { return rlabel[dom[label[x]]]; }
};
