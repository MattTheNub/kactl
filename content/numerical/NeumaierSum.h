/**
 * Author: OpenAI
 * Date: 2026-10-21
 * License: CC0
 * Source: A. Neumaier, ZAMM 54 (1974), 39--51
 * Description: More precise summation for floating
 * point types. Absolute error at most
 * $\epsilon |\sum a_i|/2 +
 * (3n^2/16+n/4)\epsilon^2\sum |a_i|$.
 * Sum must be finite and not overflow. Do not use with {\tt Ofast}.
 * Time: O(n)
 * Status: stress-tested
 */
#pragma once

template <class T> T neumaierSum(const vector<T> &a) {
	T sum = 0, c = 0;
	for (T x : a) {
		T t = sum + x;
		if (abs(sum) >= abs(x))
			c += (sum - t) + x;
		else
			c += (x - t) + sum;
		sum = t;
	}
	return sum + c;
}
