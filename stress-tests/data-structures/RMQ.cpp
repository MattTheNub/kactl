#include "../utilities/template.h"

#include "../../content/data-structures/RMQ.h"

int main() {
	srand(2);
	krep(N,0,100) {
		vi v(N);
		krep(i,0,N) v[i] = i;
		random_shuffle(all(v));
		RMQ<int> rmq(v);
		krep(i,0,N) krep(j,i+1,N+1) {
			int m = rmq.query(i,j);
			int n = 1 << 29;
			krep(k,i,j) n = min(n, v[k]);
			assert(n == m);
		}
	}
	cout<<"Tests passed!"<<endl;
}
