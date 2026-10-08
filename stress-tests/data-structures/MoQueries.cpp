#include "../utilities/template.h"

namespace MoTree { void add(int, int); void del(int, int); int calc(); }
bool treeMode = false;

int curL = 0, curR = 0, ops = 0;
void add(int ind, int end) {
	if (treeMode) return MoTree::add(ind, end);
	if (curL != curR) {
		if (end == 0) assert(ind == curL - 1);
		else assert(ind == curR);
	}
	if (curL == curR) curL = ind, curR = ind + 1;
	else if (ind == curR) curR++;
	else curL--;
	ops++;
}
void del(int ind, int end) {
	if (treeMode) return MoTree::del(ind, end);
	if (end == 0) assert(ind == curL);
	else assert(ind == curR - 1);
	if (ind == curR - 1) curR--;
	else curL++;
	assert(curL <= curR);
	ops++;
}

int calc() {
	if (treeMode) return MoTree::calc();
	return curL == curR ? -1 : curL + (curR - curL) * 10;
}

#include "../../content/data-structures/MoQueries.h"

void test(int n, int q) {
	treeMode = false;
	curL = curR = ops = 0;
	vector<pii> queries(q);
	for (auto& pa : queries) {
		pa.first = rand() % n;
		pa.second = rand() % n;
		if (pa.first > pa.second)
			swap(pa.first, pa.second);
	}
	vi res = mo(queries);
	krep(i,0,q) {
		int l = queries[i].first, r = queries[i].second;
		assert(res[i] == l + (r - l + 1) * 10);
	}

}

#undef K

namespace MoTree {

vi vals;
int sum;
deque<int> path;
void add(int i, int end) {
	sum += vals[i];
	ops++;
	if (end == 0) path.push_front(i);
	else path.push_back(i);
}
void del(int i, int end) {
	sum -= vals[i];
	ops++;
	assert(!path.empty());
	if (end == 0) {
		assert(path.front() == i);
		path.pop_front();
	} else {
		assert(path.back() == i);
		path.pop_back();
	}
}
int calc() { return sum; }


}

void testTr(int n, int q) {
	treeMode = true;
	ops = 0;
	vector<array<int, 2>> queries(q);
	for (auto& pa : queries) {
		pa[0] = rand() % n;
		pa[1] = rand() % n;
	}
	vi par(n), val(n);
	krep(i,1,n) par[i] = rand() % i;
	krep(i,0,n) val[i] = rand() % 1000;
	vector<vi> ed(n);
	krep(i,1,n) ed[par[i]].push_back(i), ed[i].push_back(par[i]);
	MoTree::vals = val;
	MoTree::sum = 0;
	MoTree::path.clear();
	vi res = moTree(queries, ed);
	vi seen(n);
	krep(i,0,q) {
		// Tree depth is logarithmic, so compute query answers naively
		int l = queries[i][0], r = queries[i][1];
		int at = l;
		while (at != 0) seen[at] = 1, at = par[at];
		seen[at] = 1;
		int sum = 0;
		while (!seen[r]) sum += val[r], r = par[r];
		at = l;
		while (at != 0) seen[at] = 0, at = par[at];
		seen[at] = 0;
		while (l != r) sum += val[l], l = par[l];
		sum += val[l];
		assert(res[i] == sum);
	}
}

int main() {
	treeMode = false;
	assert((mo({{0,-1}, {0,0}, {3,2}, {2,4}, {0,-1}}) == vi{-1,10,-1,32,-1}));
	srand(2);
	krep(it,0,10) krep(n,1,15) krep(q,0,n*n) {
		testTr(n, q);
	}
	testTr(100'000, 100'000);
	testTr(1000, 100'000);
	testTr(100'000, 1000);
	test(100'000, 100'000);
	test(1000, 100'000);
	test(100'000, 1000);
	krep(it,0,10) krep(n,1,15) krep(q,0,n*n) {
		test(n, q);
	}
	cout << "Tests passed!" << endl;
}
