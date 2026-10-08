#include "../utilities/template.h"
#include "../../content/data-structures/SegmentTree.h"

int main() {
	mt19937 rng(42);
	seg other(3);
	other.modify(1, 17);
	for (int size : {1, 2, 3, 7, 32, 129}) {
		seg tree(size);
		assert(tree.query(0, size-1) == 0);
		vector<ll> a(size);
		krep(it,0,10000) {
			int p = int(rng() % size);
			ll val = int(rng() % 100) - 50;
			tree.modify(p, val); a[p] = val;
			int l = int(rng() % size), r = int(rng() % size);
			if (l > r) swap(l, r);
			assert(tree.query(l, r) == accumulate(a.begin()+l, a.begin()+r+1, 0LL));
			assert(tree.query(p, p) == val);
			assert(tree.query(0, size-1) == accumulate(all(a), 0LL));
			assert(tree.query(size, size-1) == 0);
			assert(other.query(0, 2) == 17);
		}
	}
	cout << "Tests passed!\n";
}
