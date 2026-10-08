#include "../utilities/template.h"
#include "../../content/data-structures/LazySegmentTree.h"
const int capacity = N;
#undef N
#undef L

int main() {
	mt19937 rng(42);
	vector<ll> a(capacity);
	// Include the full, non-power-of-two capacity and its final element.
	krep(i,0,capacity) lztree::modify(i, 3);
	fill(all(a), 3);
	lztree::modify(capacity-1, 9);
	a.back() = 9;
	assert(lztree::query(0, capacity-1) == accumulate(all(a), 0LL));
	krep(it,0,10000) {
		int l = int(rng() % capacity), r = int(rng() % capacity);
		if (it % 2) l %= 37, r %= 37;
		if (l > r) swap(l, r);
		ll val = int(rng() % 100) - 50;
		if (it % 3 == 0) {
			lztree::modify(l, l, val);
			a[l] += val;
		} else if (it % 3 == 1) {
			lztree::modify(l, val); a[l] = val;
		}
		assert(lztree::query(l, r) == accumulate(a.begin()+l, a.begin()+r+1, 0LL));
		assert(lztree::query(l, l) == a[l]);
		assert(lztree::query(0, capacity-1) == accumulate(all(a), 0LL));
	}
	// Check range-update endpoints through point queries. Aggregation is
	// customizable; the supplied h does not scale additions by segment length.
	lztree::modify(0, capacity-1, 3);
	for (ll& x : a) x += 3;
	krep(it,0,1000) {
		int l = int(rng() % capacity), r = int(rng() % capacity);
		if (l > r) swap(l, r);
		ll val = int(rng() % 100) - 50;
		lztree::modify(l, r, val);
		krep(i,l,r+1) a[i] += val;
		for (int p : {0, l, r, capacity-1, max(0,l-1), min(capacity-1,r+1)})
			assert(lztree::query(p, p) == a[p]);
	}
	cout << "Tests passed!\n";
}
