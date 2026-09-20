/**
 * Author: Mateusz Klosin
 * Date: 2026-09-19
 * License: CC0
 * Description: Suffix link automaton trie for Aho-Corasick. Use {\tt go} to
 * traverse the automaton. {\tt for (int v = u; v; v = trie.next(v))} can be
 * used to iterate over all pattern matches at the current index.
 * {\tt go}, {\tt next}, and {\tt get\_link} are amortized $\mathcal O(1)$.
 * Time: $O(|\Sigma|N)$
 * Status: stress-tested
 */
#pragma once

struct AhoCorasick {
	enum {alpha = 26, first = 'a'}; // change this!
	struct Node {
		int par, link = -1, out = -1, to[alpha]{}, memo[alpha];
		char ch;
		bool end = false;
		Node(int p = -1, char c = 0) : par(p), ch(c) {
			fill(memo, memo + alpha, -1);
		}
	};
	vector<Node> N{Node()};

	// insert s into the trie and return its last node
	int insert(string s) {
		int v = 0;
		for (char c : s) {
			int i = c - first, u = N[v].to[i];
			if (!u) {
				u = sz(N);
				N[v].to[i] = u;
				N.emplace_back(v, c);
			}
			v = u;
		}
		N[v].end = true;
		return v;
	}
	// suffix link of node v
	int get_link(int v) {
		int& l = N[v].link;
		if (l == -1) l = v && N[v].par ? go(get_link(N[v].par), N[v].ch) : 0;
		return l;
	}
	// from node v, follow the transition for character c
	int go(int v, char c) {
		int i = c - first, &g = N[v].memo[i];
		if (g == -1) g = N[v].to[i] ? N[v].to[i] : v ? go(get_link(v), c) : 0;
		return g;
	}
	// longest proper suffix of node v among all patterns
	int next(int v) {
		int& u = N[v].out;
		if (u == -1) {
			int l = get_link(v);
			u = !l || N[l].end ? l : next(l);
		}
		return u;
	}
};
