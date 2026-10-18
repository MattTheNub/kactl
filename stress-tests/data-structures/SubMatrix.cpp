#include "../utilities/template.h"
#include "../../content/data-structures/SubMatrix.h"
int main() {
	mt19937 rng(7);
	rep(rows,1,10) rep(cols,1,10) {
		vector<vector<ll>> a(rows, vector<ll>(cols));
		for (auto& row : a) for (auto& x : row) x = int(rng()%100)-50;
		SubMatrix<ll> m(a);
		rep(u,0,rows) rep(d,u,rows) rep(l,0,cols) rep(r,l,cols) {
			ll sum = 0;
			rep(i,u,d+1) rep(j,l,r+1) sum += a[i][j];
			assert(m.sum(u,l,d,r) == sum);
		}
		assert(m.sum(0,0,-1,cols-1) == 0);
		assert(m.sum(0,cols,rows-1,cols-1) == 0);
	}
	cout << "Tests passed!\n";
}
