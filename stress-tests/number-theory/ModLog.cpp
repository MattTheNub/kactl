#include "../utilities/template.h"

#include "../../content/number-theory/ModLog.h"

int main() {
	const int lim = 100;
	krep(m,1,lim) {
		krep(a,0,lim) {
			vector<ll> ans(m, -1);
			ll b = a % m;
			krep(x,1,max(m,2)) {
				if (ans[b] == -1) ans[b] = x;
				b = b * a % m;
			}
			krep(b,0,m) {
				ll res = modLog(a, b, m);
				if (ans[b] != res) {
					cerr << "FAIL" << endl;
					cerr << "Expected log(" << a << ", " << b << ", " << m << ") = " << ans[b] << ", found " << res << endl;
					return 1;
				}
			}
		}
	}
	cout<<"Tests passed!"<<endl;
}
