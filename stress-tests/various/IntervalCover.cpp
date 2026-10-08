#include "../utilities/template.h"
#include "../../content/various/IntervalCover.h"
int main() {
	assert((cover(pii(0,4), vector<pii>{{0,2},{2,4}}) == vi{0,1}));
	assert(cover(pii(2,2), vector<pii>{{0,4}}).empty());
	mt19937 rng(7);
	krep(it,0,10000) {
		vector<pii> intervals(8);
		for (auto& [l,r] : intervals) {
			l = int(rng()%7); r = int(rng()%7);
			if (l > r) swap(l,r);
		}
		int l = int(rng()%7), r = int(rng()%7);
		if (l > r) swap(l,r);
		int best = 100;
		krep(mask,0,1<<8) {
			bool ok = true;
			// Half-integer samples detect real gaps between integer endpoints.
			for (int x = 2*l; x < 2*r; ++x) {
				bool hit = false;
				krep(i,0,8) if (mask>>i&1)
					hit |= 2*intervals[i].first <= x && x < 2*intervals[i].second;
				ok &= hit;
			}
			if (ok) best = min(best, __builtin_popcount(unsigned(mask)));
		}
		vi got = cover(pii(l,r), intervals);
		assert(sz(got) == (best == 100 ? 0 : best));
	}
	cout << "Tests passed!\n";
}
