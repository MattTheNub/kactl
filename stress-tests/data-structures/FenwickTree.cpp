#include "../utilities/template.h"

#include "../../content/data-structures/FenwickTree.h"

int main() {
	krep(it,0,100000) {
		int N = rand() % 10;
		FT fw(N);
		vi t(N);
		krep(i,0,N) {
			int v = rand() % 3;
			fw.update(i, v);
			t[i] += v;
		}
		assert(fw.query(-1) == 0);
		ll prefix = 0;
		krep(i,0,N) {
			prefix += t[i];
			assert(fw.query(i) == prefix);
			krep(j,0,i+1)
				assert(fw.query(i) - fw.query(j-1) == accumulate(t.begin()+j, t.begin()+i+1, 0LL));
		}
		int q = rand() % 20;
		int ind = fw.lower_bound(q);
		int res = -1, sum = 0;
		krep(i,0,N+1) {
			if (sum < q) res = i;
			if (i != N) sum += t[i];
		}
		assert(res == ind);
	}
	cout<<"Tests passed!"<<endl;
}
