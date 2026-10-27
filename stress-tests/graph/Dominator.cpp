#include "../utilities/template.h"
#include "../../content/graph/Dominator.h"

vi reachable(const vector<vi>& adj, int root, int removed) {
	vi seen(sz(adj)), q;
	if (root != removed) seen[root] = 1, q.push_back(root);
	rep(i,0,sz(q)) for (int v : adj[q[i]])
		if (v != removed && !seen[v]) {
			seen[v] = 1;
			q.push_back(v);
		}
	return seen;
}

void test(const vector<vi>& adj, int root) {
	int n = sz(adj);
	Dominator d(n);
	rep(u,0,n) for (int v : adj[u]) d.ae(u, v);
	d.calc(root);
	vi seen = reachable(adj, root, -1), count(n);
	vector<vi> dominates(n, vi(n));
	rep(u,0,n) {
		vi without = reachable(adj, root, u);
		rep(v,0,n) dominates[u][v] = seen[v] && !without[v];
		for (int v : d.ans[u]) {
			assert(0 <= v && v < n);
			assert(d.parent(v) == u);
			count[v]++;
		}
	}
	rep(v,0,n) {
		assert(count[v] == (seen[v] && v != root));
		if (!seen[v] || v == root) assert(d.parent(v) == -1);
		if (!seen[v]) continue;
		vi ancestors(n);
		for (int u = v; u != -1; u = d.parent(u)) {
			assert(0 <= u && u < n);
			assert(!ancestors[u]);
			ancestors[u] = 1;
		}
		rep(u,0,n) assert(ancestors[u] == dominates[u][v]);
	}
}

int main() {
	// Exhaust all directed graphs through four vertices,
	// including self-loops, with every choice of root.
	rep(n,1,5) rep(mask,0,1 << (n*n)) {
		vector<vi> adj(n);
		rep(u,0,n) rep(v,0,n)
			if (mask & (1 << (u*n+v))) adj[u].push_back(v);
		rep(root,0,n) test(adj, root);
	}
	// Larger graphs include parallel edges and shuffled DFS order.
	mt19937 rng(123);
	rep(it,0,10000) {
		int n = 1 + int(rng() % 40);
		int m = int(rng() % (n*n+1));
		vector<vi> adj(n);
		rep(e,0,m) {
			int u = int(rng() % n), v = int(rng() % n);
			adj[u].push_back(v);
		}
		test(adj, int(rng() % n));
	}
	// A longer DFS path, with back edges and a nonzero root.
	int n = 10000;
	Dominator d(n);
	rep(v,1,n) d.ae(v, v-1), d.ae(v-1, v);
	d.calc(n-1);
	rep(v,0,n) assert(d.parent(v) == (v == n-1 ? -1 : v+1));
	cout << "Tests passed!" << endl;
}
