#include "../utilities/template.h"
#include "../../content/various/ConstantIntervals.h"
int main() {
	mt19937 rng(7);
	krep(n,0,100) krep(it,0,100) {
		vi a(n);
		krep(i,0,n) a[i] = (i ? a[i-1] : 0) + int(rng()%2);
		int start = 0, last = -1;
		constantIntervals(0, n-1, [&](int i) {
			assert(0 <= i && i < n); return a[i];
		}, [&](int l, int r, int val) {
			assert(l == start && l <= r && r < n && val != last);
			krep(i,l,r+1) assert(a[i] == val);
			start = r+1; last = val;
		});
		assert(start == n);
	}
	cout << "Tests passed!\n";
}
