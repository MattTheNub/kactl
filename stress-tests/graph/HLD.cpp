#include "../utilities/template.h"
#include "../utilities/genTree.h"
#include "../../content/graph/HLD.h"

template<bool EDGES> void test(int n, int iters) {
	mt19937 rng(7);
	rep(it,0,iters) {
		vector<vi> adj(n);
		for (auto [u, v] : n == 1 ? vector<pii>{} : genRandomTree(n)) {
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		HLD<EDGES> hld(adj), ranges(adj);
		vi par(n, -1), depth(n);
		auto root = [&](auto&& self, int v) -> void {
			for (int u : adj[v]) if (u != par[v]) {
				par[u] = v;
				depth[u] = depth[v] + 1;
				self(self, u);
			}
		};
		root(root, 0);
		vector<ll> vals(n), added(n);
		rep(q,0,1000) {
			int u = int(rng()%n), v = int(rng()%n), a = u, b = v;
			vi path;
			while (a != b) {
				if (depth[a] < depth[b]) swap(a,b);
				path.push_back(a);
				a = par[a];
			}
			if (!EDGES) path.push_back(a);
			ll value = (ll(rng()%21)-10) * 1000000000LL;
			// The default lzseg supports point updates and range sums.
			// Edge values are stored at the child endpoint.
			if (!EDGES || u != 0) {
				hld.tree.modify(hld.pos[u], value);
				vals[u] = value;
			}
			ll expected = 0;
			for (int x : path) expected += vals[x];
			assert(hld.queryPath(u,v) == expected);
			int sub = int(rng()%n);
			expected = 0;
			rep(x,0,n) {
				int p = EDGES ? par[x] : x;
				while (p != -1 && p != sub) p = par[p];
				if (p == sub) expected += vals[x];
			}
			assert(hld.querySubtree(sub) == expected);
			// Default h does not scale additions by segment length, so
			// validate path additions through individual values.
			ranges.modifyPath(u,v,value);
			for (int x : path) added[x] += value;
			rep(x,0,n)
				assert(ranges.tree.query(ranges.pos[x], ranges.pos[x]) == added[x]);
		}
	}
}

int main() {
	srand(2);
	for (int n : {1, 2, 5, 24, 1000}) {
		test<false>(n, 10);
		test<true>(n, 10);
	}
	cout << "Tests passed!\n";
}
