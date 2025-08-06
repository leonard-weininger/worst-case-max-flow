/*
 * Anti-Random-DFS-Augmenting-Paths Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-27
 * License: CC0
 * Description: Generates a flow graph that requires exponentially 
 *   many dfs-augmenting paths to compute the maximum flow. Even
 *   when adjacency lists are randomly shuffled before the start.
 * Based on: Dean et al. (2006): https://doi.org/10.1007/11841036_26
 * Usage: Input: 2 integers from stdin: max_n, max_m
 *   Output: Graph to stdout in the format:
 *     n m
 *     from_1 to_1 cap_1
 *     ...
 *     from_m to_m cap_m
 *   Vertices are numbered from 1 to n, 1 = source, n = sink.
 */
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

const double REQUIRED_SUCCESS_PROBABILITY = 0.9;

struct Edge {
	int from, to, cap;
};

long long num_vertices(int i, int k) {
	return 3LL + i * (2LL + 3LL * k);
}

long long num_edges(int i, int k) {
	return 2LL + i * (6LL * k + 1LL);
}

long long max_capacity(int i, int k) {
	return (1LL << i);
}

long long num_augmenting_paths(int i, int k) {
	return (1LL << i);
}

double success_probability(int i, int k) {
	return std::pow(1.0 - std::pow(2.0, -k), 4 * i);
}

std::pair<int, int> best_parameters(int max_n, int max_m, int max_cap) {
	auto ok = [&](int i, int k) {
		if (num_vertices(i, k) > max_n) return false;
		if (num_edges(i, k) > max_m) return false;
		if (max_capacity(i, k) > max_cap) return false;
		if (success_probability(i, k) < REQUIRED_SUCCESS_PROBABILITY) return false;
		return true;
	};

	int best_i = 1;
	int best_k = 1;

	for (int i = 1; i <= 30; ++i) {
		for (int k = 1; k <= 30; ++k) {
			if (!ok(i, k)) continue;
			if (num_augmenting_paths(i, k) > num_augmenting_paths(best_i, best_k)) {
				best_i = i;
				best_k = k;
			}
		}
	}

	return std::make_pair(best_i, best_k);
}


int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 8);
	assert(max_m >= 9);
	assert(max_cap >= 2);

	auto [i, k] = best_parameters(max_n, max_m, max_cap);

	std::vector<Edge> edges;
	int n = 1;

	int s = ++n;
	int t = ++n;
	edges.push_back({s, t, 1});

	for (int p = 0; p < i; ++p) {
		int unit = (1 << p);

		int s2 = s;
		int t2 = t;

		std::vector<int> a(k), b(k), c(k);

		for (int j = 0; j < k; ++j) a[j] = ++n;
		for (int j = 0; j < k; ++j) b[j] = ++n;
		for (int j = 0; j < k; ++j) c[j] = ++n;

		int x = ++n;
		int y = ++n;

		for (int j = 0; j < k; ++j) {
			if (j + 1 < k) {
				edges.push_back({a[j], a[j + 1], 2 * unit});
			}
			edges.push_back({a[j], x, unit});
		}
		edges.push_back({a[k - 1], t2, unit});

		for (int j = 0; j < k; ++j) {
			if (j + 1 < k) {
				edges.push_back({b[j], b[j + 1], 2 * unit});
			}
			if (j == 0) {
				edges.push_back({y, b[j], unit});
			}
			else {
				edges.push_back({b[j], y, unit});
			}
		}
		edges.push_back({t2, b[0], unit});

		for (int j = 0; j < k; ++j) {
			if (j + 1 < k) {
				edges.push_back({c[j], c[j + 1], 2 * unit});
			}
			edges.push_back({c[j], s2, unit});
		}

		edges.push_back({x, c[0], unit});
		edges.push_back({c[k - 1], y, unit});

		s = a[0];
		t = b[k - 1];
	}

	edges.push_back({1, s, 1 << i});

	assert(n == num_vertices(i, k));
	assert((int) edges.size() == num_edges(i, k));

	// randomly permute vertices and shuffle edges
	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	std::vector<int> perm(n + 1);
	std::iota(perm.begin(), perm.end(), 0);
	std::shuffle(perm.begin() + 2, perm.end() - 1, rng);
	for (Edge& e : edges) {
		e.from = perm[e.from];
		e.to = perm[e.to];
	}
	std::shuffle(edges.begin(), edges.end(), rng);

	std::cout << n << ' ' << edges.size() << '\n';

	for (auto [u, v, w] : edges) {
		std::cout << u << ' ' << v << ' ' << w << '\n';
	}

	return 0;
}
