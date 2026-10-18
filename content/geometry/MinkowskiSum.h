/**
 * Author: unknown
 * Date: 2026-10-18
 * License: CC0
 * Source: folklore
 * Description: Minkowski sum of two convex hulls in counter-clockwise order,
 *  without repeated or redundant collinear vertices (as returned by convexHull).
 *  Returns counter-clockwise vertices starting at the lowest $(y,x)$.
 *  Empty hulls, points and segments are supported. Arithmetic must fit in ll.
 * Time: O(n+m)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

typedef Point<ll> P;
vector<P> minkowski(vector<P> a, vector<P> b) {
	int n = sz(a), m = sz(b), i = 0, j = 0;
	if (!n || !m) return {};
	for (auto p : {&a, &b}) {
		auto cmp = [](P u, P v) { return tie(u.y,u.x) < tie(v.y,v.x); };
		rotate(p->begin(), min_element(p->begin(), p->end(), cmp), p->end());
		p->push_back((*p)[0]);
	}
	vector<P> res;
	while (i < n || j < m) {
		res.push_back(a[i] + b[j]);
		int c = i == n ? -1 : j == m ? 1 :
			sgn((a[i+1] - a[i]).cross(b[j+1] - b[j]));
		i += c >= 0;
		j += c <= 0;
	}
	return res;
}
