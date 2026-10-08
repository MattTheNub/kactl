/**
 * Author: Simon Lindholm
 * License: CC0
 * Description: Add and remove intervals from a set of disjoint intervals.
 * Will merge the added interval with any overlapping intervals in the set when adding.
 * Intervals are [inclusive, inclusive] over integers; adjacent intervals are merged.
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

set<pii>::iterator addInterval(set<pii>& is, int L, int R) {
	if (L > R) return is.end();
	auto it = is.lower_bound({L, R}), before = it;
	while (it != is.end() && (ll)it->first <= (ll)R + 1) {
		R = max(R, it->second);
		before = it = is.erase(it);
	}
	if (it != is.begin() && (ll)(--it)->second + 1 >= L) {
		L = min(L, it->first);
		R = max(R, it->second);
		is.erase(it);
	}
	return is.insert(before, {L,R});
}

void removeInterval(set<pii>& is, int L, int R) {
	if (L > R) return;
	auto it = addInterval(is, L, R);
	auto r2 = it->second;
	if (it->first == L) is.erase(it);
	else { auto left = *it; is.erase(it); is.emplace(left.first, L-1); }
	if (R != r2) is.emplace(R+1, r2);
}
