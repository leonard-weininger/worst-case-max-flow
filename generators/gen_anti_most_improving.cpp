/*
 * Anti-Most-Improving-Augmenting-Paths Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-27
 * License: CC0
 * Description: Generates a flow graph on which
 *   \Omega(n * log(max_cap)) most-improving-augmenting-paths
 *   are required to compute the maximum flow.
 * Based on: Queyranne (1980): https://doi.org/10.1287/moor.5.2.258
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


int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 10);
	assert(max_m >= 17);
	assert(max_cap >= 3);

	std::vector<int> fib(46, 1);

	for (int i = 2; i <= 45; ++i) {
		fib[i] = fib[i - 1] + fib[i - 2];
	}

	int k = 1;
	while (k + 1 <= 15 && fib[3 * (k + 1)] <= max_cap) k += 1;

	std::vector<int> w(5);

	for (int i = 0; i < 3; ++i) {
		w[i] = 0;
		for (int j = 0; j < k; ++j) {
			w[i] += fib[3 * j + i];
		}
	}

	w[3] = fib[3 * k - 1];
	w[4] = fib[3 * k];

	const std::vector<std::vector<std::pair<int, int>>> EDGES = {
		{{0, 5}, {5, 6}, {1, 2}, {7, 8}, {3, 9}},
		{{0, 6}, {1, 7}, {2, 3}, {8, 9}},
		{{0, 1}, {6, 7}, {2, 8}, {3, 4}, {4, 9}},
		{{1, 6}, {8, 3}},
		{{7, 2}},
	};

	int n = 0;
	std::vector<Edge> edges;

	int l = std::min((max_n - 2) / 8, max_m / 17);

	int s = ++n;
	int t = 8 * l + 2;

	auto antiMostImproving = [&]() {
		std::vector<int> v(10);
		v[0] = s;
		v[9] = t;
		for (int i = 1; i <= 8; ++i) {
			v[i] = ++n;
		}
		for (int i = 0; i < 5; ++i) {
			for (auto [a, b] : EDGES[i]) {
				edges.push_back({v[a], v[b], w[i]});
			}
		}
	};

	for (int i = 0; i < l; ++i) {
		antiMostImproving();
	}

	assert(n + 1 == 8 * l + 2);
	assert(n + 1 <= max_n);
	assert((int) edges.size() <= max_m);

	std::cout << n + 1 << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
