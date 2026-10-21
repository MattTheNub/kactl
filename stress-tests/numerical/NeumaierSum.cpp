#include "../utilities/template.h"
#include "../../content/numerical/NeumaierSum.h"

template<class T>
void testType() {
	assert(neumaierSum<T>({}) == 0);
	assert(neumaierSum<T>({-3.5}) == -3.5);
	assert(neumaierSum<T>({0, 0, 0}) == 0);
	assert(neumaierSum<T>({1, 2, 3, 4}) == 10);
	T large = ldexp(T(1), numeric_limits<T>::digits + 10);
	vector<T> a = {large, 1, -large};
	static_assert(is_same<decltype(neumaierSum(a)), T>::value);
	assert(neumaierSum(a) == 1);
	assert(neumaierSum<T>({1, large, 1, -large}) == 2);
	assert(neumaierSum<T>({-1, -large, -1, large}) == -2);
	T tiny = numeric_limits<T>::denorm_min();
	assert(neumaierSum<T>({1, tiny, -1}) == tiny);
	T precise = 1 + numeric_limits<T>::epsilon();
	assert(neumaierSum<T>({precise}) == precise);
}

int main() {
	testType<float>();
	testType<double>();
	testType<long double>();
	assert(neumaierSum<double>({1e100, 1, -1e100}) == 1);
	assert(neumaierSum<double>({1, 1e100, 1, -1e100}) == 2);
	assert(neumaierSum<double>({-1, -1e100, -1, 1e100}) == -2);
	double tiny = numeric_limits<double>::denorm_min();
	assert(neumaierSum<double>({1, tiny, -1}) == tiny);
	vector<double> a(10000, 1);
	a.insert(a.begin(), 1e100);
	a.push_back(-1e100);
	assert(neumaierSum(a) == 10000);

	mt19937 rng(123);
	rep(it,0,10000) {
		a.clear();
		ll exact = 0;
		int n = 1 + int(rng() % 100);
		int exponent = int(rng() % 1801) - 900;
		rep(i,0,n) {
			double large = ldexp(1.0, 53 + int(rng() % 8));
			int small = int(rng() % 2001) - 1000;
			a.push_back(large);
			a.push_back(small);
			a.push_back(-large);
			exact += small;
		}
		shuffle(all(a), rng);
		for (double& x : a) x = ldexp(x, exponent);
		assert(neumaierSum(a) == ldexp(double(exact), exponent));
	}
	cout << "Tests passed!" << endl;
}
