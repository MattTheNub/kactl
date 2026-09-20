#include "../utilities/template.h"
#include "../../content/strings/AhoCorasick.h"

#define trav(a, x) for (auto& a : x)

template<class F>
void gen(string& s, int at, int alpha, F f) {
	if (at == sz(s)) f();
	else {
		krep(i,0,alpha) {
			s[at] = (char)('a' + i);
			gen(s, at+1, alpha, f);
		}
	}
}

void test(const string& s) {
	vector<string> pats;
	string cur;
	krep(i,0,sz(s)) {
		if (s[i] == 'a') {
			pats.push_back(cur);
			cur = "";
		}
		else cur += s[i];
	}

	string hay = cur;
	trav(x, pats) if (x.empty()) return;

	AhoCorasick ac;
	vector<vi> ids(1);
	krep(j,0,sz(pats)) {
		int v = ac.insert(pats[j]);
		ids.resize(sz(ac.N));
		ids[v].push_back(j);
	}
	vector<vi> positions(sz(hay));
	int v = 0;
	krep(i,0,sz(hay)) {
		v = ac.go(v, hay[i]);
		for (int u = v; u; u = ac.next(u)) if (ac.N[u].end)
			for (int j : ids[u]) positions[i - sz(pats[j]) + 1].push_back(j);
	}

	vi ord;
	krep(i,0,sz(hay)) {
		ord.clear();
		krep(j,0,sz(pats)) {
			string& pat = pats[j];
			if (hay.substr(i, pat.size()) == pat) {
				ord.push_back(j);
			}
		}
		sort(all(positions[i]));

		if (positions[i] != ord) {
			cerr << "failed!" << endl;
			cerr << hay << endl;
			trav(x, pats) cerr << x << endl;
			cerr << "failed at position " << i << endl;
			cerr << "got:" << endl;
			trav(x, positions[i]) cerr << x << ' ';
			cerr << endl;
			cerr << "expected:" << endl;
			trav(x, ord) cerr << x << ' ';
			cerr << endl;
			abort();
		}
	}
}

int main() {
	// test ~4^10 strings
	krep(n,0,11) {
		string s(n, 'x');
		gen(s, 0, 4, [&]() {
			test(s);
		});
	}
	// then ~5^7
	krep(n,0,8) {
		string s(n, 'x');
		gen(s, 0, 5, [&]() {
			test(s);
		});
	}
	cout<<"Tests passed!"<<endl;
}
