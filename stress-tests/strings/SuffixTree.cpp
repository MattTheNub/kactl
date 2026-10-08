#include "../utilities/template.h"
#include "../../content/strings/SuffixTree.h"

void suffixes(SuffixTree& t, int v, string path, set<string>& out) {
	bool leaf = true;
	krep(c,0,t.ALPHA) if (t.t[v][c] != -1) {
		leaf = false;
		int u = t.t[v][c];
		assert(0 <= t.l[u] && t.l[u] <= t.r[u] && t.r[u] < sz(t.a));
		suffixes(t, u, path + t.a.substr(t.l[u], t.r[u]-t.l[u]+1), out);
	}
	if (leaf) out.insert(path);
}
int main() {
	mt19937 rng(7);
	krep(it,0,400) {
		string a, b;
		krep(i,0,int(rng()%25)) a += char('a'+rng()%3);
		krep(i,0,int(rng()%25)) b += char('a'+rng()%3);
		string s = a + 'y' + b + 'z';
		auto t = make_unique<SuffixTree>(s);
		set<string> got, expected;
		suffixes(*t, 0, "", got);
		krep(i,0,sz(s)) expected.insert(s.substr(i));
		assert(got == expected);
		t->lcs(0, sz(a), sz(a)+1+sz(b), 0);
		int best = 0;
		krep(l,0,sz(a)) krep(r,l,sz(a))
			if (b.find(a.substr(l,r-l+1)) != string::npos) best = max(best,r-l+1);
		assert(t->best.first == best);
		if (best) {
			string sub = s.substr(t->best.second, best);
			assert(a.find(sub) != string::npos && b.find(sub) != string::npos);
		}
	}
	cout << "Tests passed!\n";
}
