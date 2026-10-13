/**
 * Author: Astra
 * Date: 2026-10-13
 * License: CC0
 * Source: Standard angle-sort and deque algorithm
 * Description:\\
\begin{minipage}{67mm}
Intersects closed half-planes to the left of directed lines $a \to b$,
with $a \ne b$. The intersection must be bounded (add bounding box edges
if needed). Returns its vertices in counter-clockwise order, or empty if
empty or of area at most eps. Nearly parallel lines can cause precision
issues.
\end{minipage}
\begin{minipage}{23mm}
\includegraphics[width=\textwidth]{content/geometry/HalfPlaneIntersection}
\end{minipage}
 * Usage: hpInter({HP(P(0,0), P(1,0)), ...});
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

typedef Point<double> P;
const double eps = 1e-9;
struct HP {
	P p, d;
	HP(P a, P b) : p(a), d((b-a).unit()) {}
	int half() const { return d.y < 0 || (d.y == 0 && d.x < 0); }
	bool operator<(HP b) const {
		return half() != b.half() ? half() < b.half() : d.cross(b.d) > 0;
	}
	bool out(P q) { return d.cross(q-p) < -eps; }
	bool parallel(HP b) { return abs(d.cross(b.d)) < eps; }
	P inter(HP b) { return p + d * (b.d.cross(p-b.p) / d.cross(b.d)); }
};

vector<P> hpInter(vector<HP> h) {
	sort(all(h));
	while (sz(h) > 1 && h[0].parallel(h.back()) && h[0].d.dot(h.back().d) > 0) {
		if (h[0].d.cross(h.back().p-h[0].p) > 0) h[0] = h.back();
		h.pop_back();
	}
	deque<HP> q;
	for (HP a : h) {
		while (sz(q) > 1 && a.out(q[sz(q)-2].inter(q.back()))) q.pop_back();
		while (sz(q) > 1 && a.out(q[0].inter(q[1]))) q.pop_front();
		if (!q.empty() && a.parallel(q.back())) {
			if (a.d.dot(q.back().d) < 0) return {};
			if (a.d.cross(q.back().p-a.p) >= 0) continue;
			q.pop_back();
		}
		q.push_back(a);
	}
	while (sz(q) > 2 && q[0].out(q[sz(q)-2].inter(q.back()))) q.pop_back();
	while (sz(q) > 2 && q.back().out(q[0].inter(q[1]))) q.pop_front();
	if (sz(q) < 3 || q[0].parallel(q.back())) return {};
	vector<P> res;
	krep(i,0,sz(q)) res.push_back(q[i].inter(q[(i+1)%sz(q)]));
	double area = 0;
	krep(i,1,sz(res)-1) area += res[0].cross(res[i], res[i+1]);
	return area > 2 * eps ? res : vector<P>{};
}
