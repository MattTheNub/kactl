#include "../utilities/template.h"

#include "../../content/geometry/HalfPlaneIntersection.h"
#include "../../content/geometry/PolygonCut.h"

using Line = pair<P, P>;
const vector<P> box = {P(-100,-100), P(100,-100), P(100,100), P(-100,100)};

double area(const vector<P>& p) {
	double a = 0;
	rep(i,1,sz(p)-1) a += p[0].cross(p[i], p[i+1]);
	return a / 2;
}

vector<Line> edges(const vector<P>& p) {
	vector<Line> res;
	rep(i,0,sz(p)) res.emplace_back(p[i], p[(i+1)%sz(p)]);
	return res;
}

mt19937 rng(12345);

void check(vector<Line> lines, double expected = -1) {
	vector<P> ref = box;
	for (auto [a,b] : lines) ref = polygonCut(ref, b, a);
	if (expected >= 0) assert(abs(area(ref) - expected) < 1e-7);
	for (auto e : edges(box)) lines.push_back(e);
	shuffle(all(lines), rng);
	vector<HP> h;
	for (auto [a,b] : lines) h.emplace_back(a, b);
	auto res = hpInter(h);
	if (abs(area(res) - area(ref)) >= 1e-6 * max(1.0, area(ref))) {
		cerr << setprecision(17) << "Areas: " << area(res) << " " << area(ref) << endl;
		for (auto [a,b] : lines) cerr << a << " -> " << b << endl;
		abort();
	}
	if (area(ref) == 0) assert(res.empty());
	for (P p : res) {
		assert(isfinite(p.x) && isfinite(p.y));
		for (auto a : h) if (!(a.d.cross(p - a.p) > -1e-6)) {
			cerr << setprecision(17) << "Outside: " << p << " distance "
				<< a.d.cross(p - a.p) << endl;
			for (auto [s,e] : lines) cerr << s << " -> " << e << endl;
			abort();
		}
	}
	rep(i,0,sz(res))
		assert(res[i].cross(res[(i+1)%sz(res)], res[(i+2)%sz(res)]) > -1e-6);
}

int main() {
	// A bounded input needs no artificial bounding box.
	vector<HP> triangle;
	for (auto [a,b] : edges({P(0,0), P(4,0), P(0,3)}))
		triangle.emplace_back(a, b);
	assert(abs(area(hpInter(triangle)) - 6) < 1e-9);
	check({}, 40000);
	check({{P(0,0), P(1,0)}}, 20000);
	check(edges({P(0,0), P(4,0), P(0,3)}), 6);
	check({{P(0,0), P(1,0)}, {P(0,1), P(2,1)},
		{P(0,1), P(7,1)}, {P(0,3), P(-1,3)}}, 400);
	// Contradictory parallels, a segment, a point, and nonparallel emptiness.
	check({{P(0,1), P(1,1)}, {P(0,0), P(-1,0)}}, 0);
	check({{P(0,0), P(1,0)}, {P(0,0), P(-1,0)}}, 0);
	check({{P(0,0), P(1,0)}, {P(0,0), P(0,-1)},
		{P(0,0), P(-1,1)}}, 0);
	check({{P(0,0), P(1,0)}, {P(0,0), P(0,-1)},
		{P(-1,0), P(-2,1)}}, 0);
	// Directions straddling the angular-order boundary and almost parallel lines.
	check({{P(0,1), P(-1,1+1e-10)}, {P(0,2), P(-1,2-1e-10)}});
	check({{P(0,0), P(-1,-0.0)}, {P(0,1), P(-2,1)}});
	check({{P(0,0), P(1,1e-8)}, {P(0,1), P(1,1+2e-8)}});
	rep(it,0,30000) {
		vector<Line> lines;
		int n = int(rng() % 30);
		rep(i,0,n) {
			auto coord = [&]() { return int(rng() % 41) - 20; };
			P a(coord(), coord()), d(coord(), coord());
			if (it % 4 >= 2) { a = a / 7; d = d / 11; }
			if (d == P()) continue;
			// Half the cases share a feasible disk about the origin.
			if (it % 2) a = d.perp() * (-1 - int(rng() % 5));
			lines.emplace_back(a, a+d);
			if (rng() % 4 == 0) lines.emplace_back(a, a+d*3);
		}
		check(lines);
	}
	// A large bounded intersection exercises every deque entry.
	vector<HP> h;
	int n = 100000;
	rep(i,0,n) {
		double a = 2 * acos(-1.0) * i / n;
		P p(cos(a), sin(a));
		h.emplace_back(p, p + p.perp());
	}
	shuffle(all(h), rng);
	auto res = hpInter(h);
	assert(sz(res) == n);
	assert(abs(area(res) - n * tan(acos(-1.0) / n)) < 1e-8);
	cout << "Tests passed!" << endl;
}
