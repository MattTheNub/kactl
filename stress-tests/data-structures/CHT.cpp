#include "../utilities/template.h"
#include "../../content/data-structures/CHT.h"

void check(vector<pair<ll, ll>> lines, vector<ll> xs, bool queryMax,
           bool interleave) {
	CHT cht(queryMax);
	vector<pair<ll, ll>> added;
	int i = 0;
	for (ll x : xs) {
		int n = interleave ? min(sz(lines), i + 1) : sz(lines);
		while (i < n) {
			auto [k, m] = lines[i++];
			cht.add(k, m);
			added.emplace_back(k, m);
		}
		ll want = queryMax ? LLONG_MIN : LLONG_MAX;
		for (auto [k, m] : added) {
			__int128 val = (__int128)k*x + m;
			assert(LLONG_MIN <= val && val <= LLONG_MAX);
			want = queryMax ? max(want, (ll)val) : min(want, (ll)val);
		}
		assert(cht.query(x) == want);
	}
}

int main() {
	mt19937 rng(123);
	for (bool queryMax : {false, true}) {
		int s = queryMax ? 1 : -1;
		for (bool interleave : {false, true}) {
			// Equal slopes, worse/identical duplicates, and tied intersections.
			vector<pair<ll, ll>> lines{{-2, 0}, {-2, -1}, {-2, 0},
				{-2, 1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}};
			for (auto& [k, m] : lines) k *= s, m *= s;
			check(lines, {-10, -5, -1, 0, 0, 0, 1, 2, 10}, queryMax, interleave);
			krep(it,0,10000) {
				lines.clear();
				vector<ll> xs;
				ll k = -100, x = -100;
				krep(j,0,100) {
					k += rng() % 4;
					x += rng() % 4;
					lines.emplace_back(s*k, (ll)(rng() % 201) - 100);
					xs.push_back(x);
				}
				check(lines, xs, queryMax, interleave);
			}
		}
		// Differences exceed ll even though coefficients and evaluations fit.
		check({{-s, -LLONG_MAX}, {0, LLONG_MAX}, {s, -LLONG_MAX}},
			{0, 0}, queryMax, false);
		check({{0, s}}, {LLONG_MIN, LLONG_MIN, LLONG_MAX}, queryMax, false);
	}
	cout << "Tests passed!" << endl;
}
