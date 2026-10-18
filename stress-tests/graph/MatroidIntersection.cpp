#include "../utilities/template.h"
#include "../utilities/random.h"
#include "../../content/graph/MatroidIntersection.h"
#include "../../content/graph/HopcroftKarp.h"

struct Partition {
	vi group, cap, count;
	Partition(vi g, vi c) : group(g), cap(c) { clear(); }
	void clear() { count.assign(sz(cap), 0); }
	bool check(int x) { return count[group[x]] < cap[group[x]]; }
	void add(int x) { count[group[x]]++; }
};

struct Graphic {
	vector<pii> edges;
	vi comp;
	Graphic(int n, vector<pii> e) : edges(e), comp(n) { clear(); }
	void clear() { iota(all(comp), 0); }
	bool check(int x) {
		auto [u, v] = edges[x];
		return comp[u] != comp[v];
	}
	void add(int x) {
		auto [u, v] = edges[x];
		int a = comp[u], b = comp[v];
		for (int& c : comp) if (c == a) c = b;
	}
};

struct Linear {
	vi values, basis;
	Linear(int d, vi v) : values(v), basis(d) {}
	void clear() { fill(all(basis), 0); }
	int reduce(int x) {
		for (int b : basis) x = min(x, x ^ b);
		return x;
	}
	bool check(int x) { return reduce(values[x]) != 0; }
	void add(int x) {
		int v = reduce(values[x]);
		assert(v);
		basis[__builtin_clz((unsigned)v) - (32 - sz(basis))] = v;
	}
};

// Check valid additions and record the set left in each oracle.
template<class M> struct Tracked : M {
	vi contents;
	Tracked(M m) : M(m) { clear(); }
	void clear() { M::clear(); contents.clear(); }
	void add(int x) {
		assert(find(all(contents), x) == contents.end());
		assert(M::check(x));
		M::add(x);
		contents.push_back(x);
	}
};

template<class M1, class M2>
int brute(M1 a, M2 b, int n) {
	int best = 0;
	rep(mask,0,1 << n) {
		if (__builtin_popcount((unsigned)mask) <= best) continue;
		a.clear(); b.clear();
		bool ok = true;
		rep(x,0,n) if (mask >> x & 1) {
			if (!a.check(x) || !b.check(x)) { ok = false; break; }
			a.add(x); b.add(x);
		}
		if (ok) best = __builtin_popcount((unsigned)mask);
	}
	return best;
}

template<class M1, class M2>
void verify(M1& a, M2& b, int n, int want) {
	vi ans = matroidIntersection(a, b, n);
	assert(sz(ans) == want);
	assert(is_sorted(all(ans)));
	assert(adjacent_find(all(ans)) == ans.end());
	for (int x : ans) assert(0 <= x && x < n);
	sort(all(a.contents)); sort(all(b.contents));
	assert(a.contents == ans && b.contents == ans);
	a.clear(); b.clear();
	for (int x : ans) { a.add(x); b.add(x); }
}

template<class M1, class M2>
void test(M1 m1, M2 m2, int n, int want = -1) {
	if (want == -1) want = brute(m1, m2, n);
	Tracked<M1> a(m1);
	Tracked<M2> b(m2);
	// Start with independently populated oracles, then reuse them.
	for (int x = n; x--;) {
		if (a.check(x)) a.add(x);
		if (b.check(x)) b.add(x);
	}
	verify(a, b, n, want);
	verify(b, a, n, want);
}

int main() {
	// Empty ground set; rank zero; free and uniform matroids.
	test(Partition({}, {}), Partition({}, {}), 0, 0);
	rep(n,1,15) rep(a,0,n + 1) rep(b,0,n + 1)
		test(Partition(vi(n), {a}), Partition(vi(n), {b}),
			n, min(a, b));
	// Greedy takes (0,0); optimum must replace it by (0,1),(1,0).
	test(Partition({0,0,1}, {1,1}),
		Partition({0,1,0}, {1,1}), 3, 2);
	// Force a final augmenting path through every selected element.
	rep(k,1,40) {
		vi left, right;
		rep(i,0,k) left.push_back(i), right.push_back(i);
		rep(i,0,k + 1) {
			left.push_back(i);
			right.push_back((i + 1) % (k + 1));
		}
		test(Partition(left, vi(k + 1, 1)),
			Partition(right, vi(k + 1, 1)), sz(left), k + 1);
	}
	srand(123456);
	rep(it,0,1500) {
		int n = randIncl(0, 11);
		auto partition = [&]() {
			int k = randIncl(1, 5);
			vi group(n), cap(k);
			for (int& x : group) x = randRange(k);
			for (int& x : cap) x = randIncl(0, 3);
			return Partition(group, cap);
		};
		auto graphic = [&]() {
			int k = randIncl(1, 7);
			vector<pii> edges(n);
			for (auto& [u, v] : edges)
				u = randRange(k), v = randRange(k);
			return Graphic(k, edges); // Includes loops/parallel edges.
		};
		auto linear = [&]() {
			int d = randIncl(1, 7);
			vi values(n);
			for (int& x : values) x = randRange(1 << d);
			return Linear(d, values); // Includes zero/duplicate vectors.
		};
		auto p = partition(); auto g = graphic(); auto l = linear();
		test(p, partition(), n);
		test(p, g, n);
		test(p, l, n);
		test(g, graphic(), n);
		test(g, l, n);
		test(l, linear(), n);
	}
	// Larger matching instances, checked with a separate algorithm.
	rep(it,0,200) {
		int l = randIncl(1, 30), r = randIncl(1, 30);
		int n = randIncl(0, 200);
		vi left(n), right(n), match(r, -1);
		vector<vi> adj(l);
		rep(x,0,n) {
			left[x] = randRange(l); right[x] = randRange(r);
			adj[left[x]].push_back(right[x]);
		}
		test(Partition(left, vi(l, 1)), Partition(right, vi(r, 1)),
			n, hopcroftKarp(adj, match));
	}
	cout << "Tests passed!" << endl;
}
