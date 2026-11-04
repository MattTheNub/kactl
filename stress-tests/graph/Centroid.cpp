#include "../utilities/template.h"
#include "../../content/graph/Centroid.h"

void verify(const vector<vi>& g) {
	int n = sz(g);
	Centroid cd(g);
	assert(cd.adj == g);
	assert(sz(cd.par) == n);
	if (!n) return;
	vector<vi> children(n);
	int root = -1;
	rep(v,0,n) {
		int p = cd.par[v];
		if (p == -1) {
			assert(root == -1);
			root = v;
		} else {
			assert(0 <= p && p < n && p != v);
			children[p].push_back(v);
		}
	}
	assert(root != -1);
	// Independently flood-fill the components left by each centroid.
	vi removed(n), seen(n), component(n, -1);
	int stamp = 0, visited = 0;
	auto check = [&](auto self, int c, int size) -> void {
		assert(!removed[c]);
		removed[c] = 1;
		visited++;
		vi sizes;
		int total = 1;
		++stamp;
		for (int u : g[c]) if (!removed[u]) {
			vi q{u};
			seen[u] = stamp;
			rep(i,0,sz(q)) {
				int v = q[i];
				component[v] = sz(sizes);
				for (int w : g[v])
					if (!removed[w] && seen[w] != stamp) {
						seen[w] = stamp;
						q.push_back(w);
					}
			}
			assert(sz(q) <= size/2);
			total += sz(q);
			sizes.push_back(sz(q));
		}
		assert(total == size);
		assert(sz(children[c]) == sz(sizes));
		vi used(sz(sizes));
		vector<pii> next;
		for (int v : children[c]) {
			assert(!removed[v] && seen[v] == stamp);
			int id = component[v];
			assert(!used[id]++);
			next.emplace_back(v, sizes[id]);
		}
		for (auto [v, s] : next) self(self, v, s);
	};
	check(check, root, n);
	assert(visited == n);
}

int main() {
	verify({});
	mt19937 rng(42);
	rep(it,0,10000) {
		int n = int(rng() % 100) + 1;
		vector<vi> g(n);
		vi perm(n);
		iota(all(perm), 0);
		shuffle(all(perm), rng);
		rep(i,1,n) {
			int u = perm[i], v = perm[rng() % i];
			g[u].push_back(v);
			g[v].push_back(u);
		}
		for (auto& adj : g) shuffle(all(adj), rng);
		verify(g);
	}
	// Paths (including two possible centroids), stars, binary trees.
	for (int n : {1, 2, 3, 4, 99, 100, 200000}) rep(type,0,3) {
		vector<vi> g(n);
		rep(i,1,n) {
			int p = type == 0 ? i-1 : type == 1 ? 0 : (i-1)/2;
			g[p].push_back(i);
			g[i].push_back(p);
		}
		verify(g);
	}
	cout << "Tests passed!" << endl;
}
