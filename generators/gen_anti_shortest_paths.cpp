/*
 * Anti-Shortest-Augmenting-Paths Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-26
 * License: CC0
 * Description: Generates a flow graph that requires \Omega(n * m)
 *   shortest augmenting paths to compute the maximum flow.
 *   The graphs are worst-case graphs for:
 *      - Edmonds-Karp: \Omega(n * m^2) time
 *      - Dinic: \Omega(n^2 * m) time.
 * Based on: Zadeh (1972): https://doi.org/10.1145/321679.321693
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

long long num_vertices(int k, int l) {
	return 4LL * l + 2LL + 2LL * k;
}

long long num_edges(int k, int l) {
	return 6LL * l - 4LL + 4LL * k + 1LL * k * k;
}

long long max_capacity(int k, int l) {
	return 1LL * k * k * l;
}

long long num_augmenting_paths(int k, int l) {
	return 1LL * k * k * l;
}

std::pair<int, int> best_parameters(int max_n, int max_m, int max_cap) {
	auto ok = [&](int k, int l) {
		if (num_vertices(k, l) > max_n) return false;
		if (num_edges(k, l) > max_m) return false;
		if (max_capacity(k, l) > max_cap) return false;
		return true;
	};

	int best_k = 1;
	int best_l = 1;

	for (int k = 1, l = 1; k <= max_n; ++k) {
		while (ok(k, l + 1)) l += 1;
		while (l >= 1 && !ok(k, l)) l -= 1;
		if (l <= 0) break;
		if (num_augmenting_paths(k, l) > num_augmenting_paths(best_k, best_l)) {
			best_k = k;
			best_l = l;
		}
	}

	return std::make_pair(best_k, best_l);
}

int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 7);
	assert(max_m >= 5);
	assert(max_cap >= 1);
	assert(1LL * max_n * max_m <= 2'000'000'000LL); // avoid overflow

	auto [k, l] = best_parameters(max_n, max_m, max_cap);
	int flow = num_augmenting_paths(k, l);

	int n = 0;
	std::vector<Edge> edges;

	std::vector<int> s(2 * l - 1);

	for (int i = 0; i < 2 * l - 1; ++i) {
		s[i] = ++n;
	}

	int p1 = ++n;
	int p2 = ++n;

	int q1 = ++n;
	int q2 = ++n;

	std::vector<int> a(k), b(k);

	for (int i = 0; i < k; ++i) {
		a[i] = ++n;
		b[i] = ++n;
	}

	std::vector<int> t(2 * l - 1);

	for (int i = 2 * l - 2; i >= 0; --i) {
		t[i] = ++n;
	}

	for (int i = 0; i < 2 * l - 2; ++i) {
		edges.push_back({s[i], s[i + 1], flow});
		edges.push_back({t[i + 1], t[i], flow});
	}

	for (int i = 0; i < 2 * l - 1; i += 2) {
		edges.push_back({s[i], (i % 4 == 0 ? p1 : p2), k * k});
		edges.push_back({(i % 4 == 0 ? q1 : q2), t[i], k * k});
	}

	for (int i = 0; i < k; ++i) {
		edges.push_back({p1, a[i], flow});
		edges.push_back({p2, b[i], flow});
		edges.push_back({a[i], q2, flow});
		edges.push_back({b[i], q1, flow});
	}

	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < k; ++j) {
			edges.push_back({a[i], b[j], 1});
		}
	}

	assert(n == num_vertices(k, l));
	assert((int) edges.size() == num_edges(k, l));

	std::cout << n << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
