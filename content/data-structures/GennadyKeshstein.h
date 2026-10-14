/**
 * Author: unknown
 * Description: Finds the maximum right endpoint after a given left
 * endpoint such that the interval satisfies a monotonic condition.
 * Requires Node, st, push(i), and pair aliases a/b for first/second.
 * Bounds are inclusive; pr is the aggregate preceding this segment.
 * Time: O(\log N)
 * Status: untested
 */
#pragma once

pair<int, Node> walk(int le, function<bool(Node)> &pred,
                    Node pr, int i, int l, int r) {
	if (le == l) {
		Node v = pr + st[i];
		if (pred(v))
			return {r, v};
	}
	if (l == r)
		return {l - 1, pr};
	push(i);
	int m = (l + r) / 2;
	if (le > m)
		return walk(le, pred, pr, i << 1 | 1, m + 1, r);
	auto p = walk(le, pred, pr, i << 1, l, m);
	if (p.a == m)
		p = walk(m + 1, pred, p.b, i << 1 | 1, m + 1, r);
	return p;
}
