/*
 * Anti-Scaling-Dinic Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-27
 * License: CC0
 * Description: Generates a flow graph on which the scaling
 *   alorithm requires \Omega(m * log(max_cap)) shortest-paths to
 *   compute the maximum flow.
 * Usage: Input: 2 integers from stdin: max_n, max_m
 *   Output: Graph to stdout in the format:
 *     n m
 *     from_1 to_1 cap_1
 *     ...
 *     from_m to_m cap_m
 *   Vertices are numbered from 1 to n, 1 = source, n = sink.
 */
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

struct Edge {
	int from, to, cap;
};

long long num_vertices(int k, int l, int p, int w) {
	return 3LL + 2LL * w + p * (4LL * l + 2LL + 2LL * k);
}

long long num_edges(int k, int l, int p, int w) {
	return 2 * w + 1LL * w * w + 2 * p + p * (6LL * l - 4LL + 4LL * k + 1LL * k * k);
}

long long max_capacity(int k, int l, int p, int w) {
	return 1LL * w * w * ((1LL << p) - 1);
}

long long estimated_work(int k, int l, int p, int w) {
	return 1LL * p * w * w * num_vertices(k, l, p, w);
}

std::array<int, 4> best_parameters(int max_n, int max_m, int max_cap) {
	auto ok = [&](int k, int l, int p, int w) {
		if (num_vertices(k, l, p, w) > max_n) return false;
		if (num_edges(k, l, p, w) > max_m) return false;
		if (max_capacity(k, l, p, w) > max_cap) return false;
		return true;
	};

	int best_k = 1;
	int best_l = 1;
	int best_p = 1;
	int best_w = 1;

	auto get_valid_kl = [&](int p, int w) {
		for (int k = 1, l = 1; ok(k, 1, p, w); ++k) {
			while (ok(k, l, p, w)) l += 1;
			while (l >= 1 && !ok(k, l, p, w)) l -= 1;
			if (l == 0) break;
			if (1LL * k * k * l >= 1LL * w * w) return std::make_pair(k, l);
		}
		return std::make_pair(-1, -1);
	};

	for (int p = 1, w = 1; p <= 29; ++p) {
		while (true) {
			auto [k, l] = get_valid_kl(p, w);
			if (k == -1 || l == -1) break;
			w += 1;
		}
		while (w >= 1) {
			auto [k, l] = get_valid_kl(p, w);
			if (k != -1 && l != -1) break;
			w -= 1;
		}
		if (w == 0) break;
		auto [k, l] = get_valid_kl(p, w);
		if (estimated_work(k, l, p, w) > estimated_work(best_k, best_l, best_p, best_w)) {
			best_k = k;
			best_l = l;
			best_p = p;
			best_w = w;
		}
	}

	return std::array<int, 4>{best_k, best_l, best_p, best_w};
}

int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 13);
	assert(max_m >= 12);
	assert(max_cap >= 1);

	auto [k, l, p, w] = best_parameters(max_n, max_m, max_cap);

	int n = 0;
	std::vector<Edge> edges;

	int s = ++n;
	int x = ++n;

	std::vector<int> a(w), b(w);

	for (int i = 0; i < w; ++i) {
		a[i] = ++n;
		b[i] = ++n;
		edges.push_back({s, a[i], max_cap});
		edges.push_back({b[i], x, max_cap});
	}

	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < w; ++j) {
			edges.push_back({a[i], b[j], (1 << p) - 1});
		}
	}

	int t = num_vertices(k, l, p, w);

	for (int iter = p - 1; iter >= 0; --iter) {
		int unit = (1 << iter);

		std::vector<int> vs(2 * l - 1);

		for (int i = 0; i < 2 * l - 1; ++i) {
			vs[i] = ++n;
		}

		int p1 = ++n;
		int p2 = ++n;

		int q1 = ++n;
		int q2 = ++n;

		std::vector<int> va(k), vb(k);

		for (int i = 0; i < k; ++i) {
			va[i] = ++n;
			vb[i] = ++n;
		}

		std::vector<int> vt(2 * l - 1);

		for (int i = 2 * l - 2; i >= 0; --i) {
			vt[i] = ++n;
		}

		for (int i = 0; i < 2 * l - 2; ++i) {
			edges.push_back({vs[i], vs[i + 1], max_cap});
			edges.push_back({vt[i + 1], vt[i], max_cap});
		}

		int need = w * w * unit;

		for (int i = 0; i < 2 * l - 1; i += 2) {
			int cap = std::min(k * k * unit, need);
			if (cap >= 1) {
				edges.push_back({vs[i], (i % 4 == 0 ? p1 : p2), cap});
				edges.push_back({(i % 4 == 0 ? q1 : q2), vt[i], cap});
				need -= cap;
			}
		}

		for (int i = 0; i < k; ++i) {
			edges.push_back({p1, va[i], max_cap});
			edges.push_back({p2, vb[i], max_cap});
			edges.push_back({va[i], q2, max_cap});
			edges.push_back({vb[i], q1, max_cap});
		}

		for (int i = 0; i < k; ++i) {
			for (int j = 0; j < k; ++j) {
				edges.push_back({va[i], vb[j], unit});
			}
		}

		edges.push_back({x, vs[0], max_cap});
		edges.push_back({vt[0], t, max_cap});

		x = vs[2 * l - 2];
		t = vt[2 * l - 2];
	}

	assert(n + 1 == num_vertices(k, l, p, w));
	assert((int) edges.size() <= num_edges(k, l, p, w));

	std::cout << n + 1 << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
