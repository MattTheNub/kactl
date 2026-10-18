#include "../utilities/template.h"
#include "../../content/geometry/MinkowskiSum.h"
#include "../../content/geometry/ConvexHull.h"

void check(vector<P> a, vector<P> b) {
	vector<P> sums;
	for (P p : a) for (P q : b) sums.push_back(p + q);
	auto want = convexHull(sums), got = minkowski(a, b);
	assert(convexHull(got) == want);
	assert(sz(got) == sz(want));
	assert(sz(got) <= sz(a) + sz(b));
	if (got.empty()) return;
	auto cmp = [](P p, P q) { return tie(p.y,p.x) < tie(q.y,q.x); };
	assert(got[0] == *min_element(all(got), cmp));
	if (sz(got) > 1) {
		krep(i,0,sz(got)) {
			P p = got[i], q = got[(i+1) % sz(got)];
			assert(!(p == q));
			for (P r : got) assert(p.cross(q, r) >= 0);
		}
	}
}

int main() {
	vector<vector<P>> cases{
		{}, {P(2,3)}, {P(0,0), P(2,0)}, {P(0,0), P(0,2)},
		{P(0,0), P(2,2)}, {P(0,2), P(2,0)},
		{P(0,0), P(2,0), P(0,2)},
		{P(0,0), P(2,0), P(2,2), P(0,2)},
		{P(-100000000,0), P(0,-100000000), P(100000000,0), P(0,100000000)}
	};
	for (auto a : cases) for (auto b : cases) {
		krep(i,0,max(1,sz(a))) {
			krep(j,0,max(1,sz(b))) {
				check(a, b);
				if (!b.empty()) rotate(b.begin(), b.begin()+1, b.end());
			}
			if (!a.empty()) rotate(a.begin(), a.begin()+1, a.end());
		}
	}
	mt19937 rng(123);
	krep(it,0,100000) {
		vector<P> a, b;
		for (auto p : {&a, &b}) {
			int n = (int)(rng() % 20);
			krep(i,0,n) p->emplace_back((ll)(rng() % 21)-10, (ll)(rng() % 21)-10);
			*p = convexHull(*p);
			if (!p->empty()) rotate(p->begin(), p->begin()+rng()%p->size(), p->end());
		}
		check(a, b);
	}
	cout << "Tests passed!" << endl;
}
