#include "../utilities/template.h"

#include "../../content/number-theory/ModularArithmetic.h"

int main() {
	krep(a,0,mod) krep(b,1,mod) {
		Mod ma(a);
		Mod mb(b);
		Mod mc = ma / mb;
		assert((mc * mb).x == a);
	}
	Mod a = 2;
	ll cur=1;
	krep(i, 0, 17) {
		assert((a ^ i).x == cur);
		cur = (cur * 2) % mod;
		// cout << i << ": " << (a ^ i).x << endl;
	}
	cout<<"Tests passed!"<<endl;
}
