/*
 * Anti-DFS-Augmenting-Paths Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-27
 * License: CC0
 * Description: Generates a flow graph that requires exponentially 
 *   many dfs-augmenting paths to compute the maximum flow.
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
#include <iostream>
#include <vector>

struct Edge {
	int from, to, cap;
};

int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 2);
	assert(max_m >= 1);
	assert(max_cap >= 1);

	if (max_n < 4 || max_m < 5) {
		std::cout << 2 << ' ' << 1 << '\n';
		std::cout << 1 << ' ' << 2 << ' ' << 1 << '\n';
		return 0;
	}

	int n = 1;
	std::vector<Edge> edges;

	int cur_s = ++n;
	int cur_t = ++n;
	int cur_cap = 1;
	edges.push_back({cur_s, cur_t, cur_cap});

	while (true) {
		bool last_iteration = false;
		if (n + 3 > max_n) last_iteration = true;
		if ((int) edges.size() + 8 > max_m) last_iteration = true;
		if (2LL * cur_cap > max_cap) last_iteration = true;

		int next_s = (last_iteration ? 1 : ++n);
		int next_t = ++n;

		edges.push_back({next_s, cur_s, cur_cap});
		edges.push_back({next_s, cur_t, cur_cap});
		edges.push_back({cur_s, next_t, cur_cap});
		edges.push_back({cur_t, next_t, cur_cap});

		if (last_iteration) break;

		cur_s = next_s;
		cur_t = next_t;
		cur_cap *= 2;
	}

	std::cout << n << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
