#include "../utilities/template.h"
#include <cassert>

namespace ignore {
#include "../../content/number-theory/ModPow.h"
}
ll modpow(ll a, ll e);
#include "../../content/numerical/BostanMori.h"
#include "../../content/numerical/Newton.h"
ll modpow(ll a, ll e) {
	ll r = 1;
	for (; e; e /= 2, a = a * a % mod)
		if (e & 1) r = r * a % mod;
	return r;
}

vl mul(const vl& a, const vl& b, int n) {
	vl c(n);
	krep(i,0,min(sz(a),n)) krep(j,0,min(sz(b),n-i))
		c[i+j] = (c[i+j] + a[i] * b[j]) % mod;
	return c;
}

vl divide(const vl& p, const vl& q, int n) {
	vl a(n);
	ll inv = modpow(q[0], mod - 2);
	krep(i,0,n) {
		a[i] = i < sz(p) ? p[i] : 0;
		krep(j,1,min(sz(q),i+1))
			a[i] = (a[i] - q[j] * a[i-j] % mod + mod) % mod;
		a[i] = a[i] * inv % mod;
	}
	return a;
}

pair<ll, ll> fib(ll k) {
	if (!k) return {0, 1};
	auto [a, b] = fib(k / 2);
	ll c = a * ((2*b - a + mod) % mod) % mod;
	ll d = (a*a + b*b) % mod;
	return k & 1 ? make_pair(d, (c+d) % mod) : make_pair(c, d);
}

int main() {
	mt19937 rng(123);
	auto randomPoly = [&](int n) {
		vl a(n);
		for (ll& x : a) x = rng() % mod;
		return a;
	};
	assert(polyInv({}, 0).empty());
	assert(bostanMori({}, {1}, LLONG_MAX) == 0);
	krep(it,0,2000) {
		int n = int(rng() % 130);
		vl q = randomPoly(1 + int(rng() % 60));
		q[0] = 1 + rng() % (mod - 1);
		if (it % 3 == 0) q.back() = 0;
		if (!q[0]) q[0] = 1;
		assert(polyInv(q, n) == divide({1}, q, n));
		vl p = randomPoly(int(rng() % 90));
		vl a = divide(p, q, n + 1);
		assert(bostanMori(p, q, n) == a[n]);
		assert(bostanMori(p, q, 0) == a[0]);

		vl root = randomPoly(max(n, 1));
		root[0] = 1 + rng() % (mod - 1);
		int power = 2 + it % 2;
		vl target = mul(root, root, n);
		if (power == 3) target = mul(target, root, n);
		a = newton(root[0], n, [&](const vl& b, int m) {
			vl val = conv(b, b), der = b;
			if (power == 3) der = val, val = conv(val, b);
			val.resize(m);
			krep(i,0,m) val[i] = (val[i] - target[i] + mod) % mod;
			for (ll& x : der) x = x * power % mod;
			return make_pair(val, der);
		});
		root.resize(n);
		assert(a == root);
	}
	for (int n : {1, 2, 3, 63, 64, 65, 1023, 1024, 1025}) {
		vl q = randomPoly(n);
		q[0] = mod - 1;
		vl expected(n);
		expected[0] = 1;
		assert(mul(q, polyInv(q, n), n) == expected);
		vl a = newton(1, n, [&](const vl& b, int m) {
			vl val = conv(b, b), der = b;
			val.resize(m), der.resize(m);
			krep(i,0,sz(b)) {
				val[i] = (val[i] - b[i] + mod) % mod;
				if (i+1 < m) val[i+1] = (val[i+1] - b[i] + mod) % mod;
				der[i] = 2 * b[i] % mod;
			}
			val[1] = (val[1] + mod - 1) % mod;
			der[0] = (der[0] + mod - 1) % mod;
			der[1] = (der[1] + mod - 1) % mod;
			return make_pair(val, der);
		});
		vl residual = mul(a, a, n);
		krep(i,0,n) {
			ll want = (a[i] + (i ? a[i-1] : 0) + (i == 1)) % mod;
			assert(residual[i] == want);
		}
	}
	for (ll k : {0LL, 1LL, 2LL, 1000000000000LL, LLONG_MAX}) {
		assert(bostanMori({0, 1}, {1, mod-1, mod-1}, k) == fib(k).first);
		assert(bostanMori({7}, {1, mod-3}, k) == 7 * modpow(3, k) % mod);
		assert(bostanMori({0, 0, 5}, {2}, k) == (k == 2 ? 5 * modpow(2, mod-2) % mod : 0));
	}
	cout << "Tests passed!" << endl;
}
