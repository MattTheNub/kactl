#include "../utilities/template.h"
const int NN = 129;
#include "../../content/data-structures/SegmentTree.h"

int main() {
	mt19937 rng(42);
	for (int size : {1, 2, 3, 7, 32, 129}) {
		seg::n = size;
		fill(begin(seg::t), end(seg::t), 0);
		vector<ll> a(size);
		krep(it,0,10000) {
			int p = int(rng() % size);
			ll val = int(rng() % 100) - 50;
			seg::modify(p, val); a[p] = val;
			int l = int(rng() % size), r = int(rng() % size);
			if (l > r) swap(l, r);
			assert(seg::query(l, r) == accumulate(a.begin()+l, a.begin()+r+1, 0LL));
			assert(seg::query(p, p) == val);
			assert(seg::query(0, size-1) == accumulate(all(a), 0LL));
			assert(seg::query(size, size-1) == 0);
		}
	}
	cout << "Tests passed!\n";
}
