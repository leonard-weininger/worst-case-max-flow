/*
 * Anti-Scaling-DFS Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-27
 * License: CC0
 * Description: Generates a flow graph on which the scaling
 *   alorithm requires \Omega(m * log(max_cap)) dfs-paths to
 *   compute the maximum flow.
 * Usage: Input: 2 integers from stdin: max_n, max_m
 *   Output: Graph to stdout in the format:
 *     n m
 *     from_1 to_1 cap_1
 *     ...
 *     from_m to_m cap_m
 *   Vertices are numbered from 1 to n, 1 = source, n = sink.
 */
#include <cassert>
#include <iostream>
#include <vector>

struct Edge {
	int from, to, cap;
};

int log2_ceil(long long x) {
	int ans = 0;
	while ((1LL << ans) < x) ans += 1;
	return ans;
}

long long num_vertices(int p, int w) {
	return 3LL + 2LL * w + 1LL * p * (log2_ceil(1LL * w * w) * 2LL + 2LL);
}

long long num_edges(int p, int w) {
	return 2LL * w + 1LL * w * w + 2LL * p + 1LL * p * (log2_ceil(1LL * w * w) * 4LL + 1LL);
}

long long max_capacity(int p, int w) {
	return std::max(1LL * w * ((1LL << p) - 1LL), 1LL * w * w * (1LL << (p - 1)));
}

long long num_augmenting_paths(int p, int w) {
	return 1LL * p * w * w;
}

std::pair<int, int> best_parameters(int max_n, int max_m, int max_cap) {
	auto ok = [&](int p, int w) {
		if (num_vertices(p, w) > max_n) return false;
		if (num_edges(p, w) > max_m) return false;
		if (max_capacity(p, w) > max_cap) return false;
		return true;
	};

	int best_p = 1;
	int best_w = 1;

	for (int p = 1; p <= 29; ++p) {
		for (int w = 1; 1LL * w * w <= max_m && max_capacity(p, w) <= max_cap; ++w) {
			if (!ok(p, w)) break;
			if (num_augmenting_paths(p, w) > num_augmenting_paths(best_p, best_w)) {
				best_p = p;
				best_w = w;
			}
		}
	}

	return std::make_pair(best_p, best_w);
}

std::pair<int, int> anti_dfs_graph(int need_paths, int unit_cap, int& n, std::vector<Edge>& edges) {
	int cur_s = ++n;
	int cur_t = ++n;
	int cur_cap = unit_cap;
	edges.push_back({cur_s, cur_t, cur_cap});

	int have_paths = 1;

	while (have_paths < need_paths) {
		int next_s = ++n;
		int next_t = ++n;

		edges.push_back({next_s, cur_s, cur_cap});
		edges.push_back({next_s, cur_t, cur_cap});
		edges.push_back({cur_s, next_t, cur_cap});
		edges.push_back({cur_t, next_t, cur_cap});

		cur_s = next_s;
		cur_t = next_t;
		cur_cap *= 2;
		have_paths *= 2;
	}

	return std::make_pair(cur_s, cur_t);
}

int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 7);
	assert(max_m >= 6);
	assert(max_cap >= 1);

	auto [p, w] = best_parameters(max_n, max_m, max_cap);

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

	std::vector<int> vt;

	for (int i = p - 1; i >= 0; --i) {
		auto [s2, t2] = anti_dfs_graph(w * w, (1 << i), n, edges);
		edges.push_back({x, s2, w * w * (1 << i)});
		vt.push_back(t2);
	}

	int t = ++n;

	for (int t2 : vt) {
		edges.push_back({t2, t, max_cap});
	}

	assert(n == num_vertices(p, w));
	assert((int) edges.size() == num_edges(p, w));

	std::cout << n << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
