#include "../utilities/template.h"
#include "../../content/data-structures/LazySegmentTree.h"

void test(int capacity) {
	mt19937 rng(42);
	lzseg tree(capacity), other(3);
	other.modify(1, 17);
	other.modify(0, 2, 5);
	assert(tree.query(0, capacity-1) == 0);
	vector<ll> a(capacity);
	// Include the full, non-power-of-two capacity and its final element.
	krep(i,0,capacity) tree.modify(i, 3);
	fill(all(a), 3);
	tree.modify(capacity-1, 9);
	a.back() = 9;
	assert(tree.query(0, capacity-1) == accumulate(all(a), 0LL));
	krep(it,0,10000) {
		int l = int(rng() % capacity), r = int(rng() % capacity);
		if (it % 2) l %= 37, r %= 37;
		if (l > r) swap(l, r);
		ll val = int(rng() % 100) - 50;
		if (it % 3 == 0) {
			tree.modify(l, l, val);
			a[l] += val;
		} else if (it % 3 == 1) {
			tree.modify(l, val); a[l] = val;
		}
		assert(tree.query(l, r) == accumulate(a.begin()+l, a.begin()+r+1, 0LL));
		assert(tree.query(l, l) == a[l]);
		assert(tree.query(0, capacity-1) == accumulate(all(a), 0LL));
	}
	// Check range-update endpoints through point queries. Aggregation is
	// customizable; the supplied h does not scale additions by segment length.
	tree.modify(0, capacity-1, 3);
	for (ll& x : a) x += 3;
	krep(it,0,1000) {
		int l = int(rng() % capacity), r = int(rng() % capacity);
		if (l > r) swap(l, r);
		ll val = int(rng() % 100) - 50;
		tree.modify(l, r, val);
		krep(i,l,r+1) a[i] += val;
		for (int p : {0, l, r, capacity-1, max(0,l-1), min(capacity-1,r+1)})
			assert(tree.query(p, p) == a[p]);
	}
	for (int p = 0; p < 3; ++p)
		assert(other.query(p, p) == (p == 1 ? 22 : 5));
}

int main() {
	for (int size : {1, 2, 3, 7, 32, 129, 100010}) test(size);
	cout << "Tests passed!\n";
}
