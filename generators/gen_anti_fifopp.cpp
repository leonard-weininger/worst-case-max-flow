/*
 * Anti-FIFO-Preflow-Push Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-27
 * License: CC0
 * Description: Generates a flow graph on which the
 *   FIFO-Preflow-Push algorithm requires \Omega(n^3) time
 *   to compute the maximum flow. 
 *   Only works if FIFOPP prioritizes edges, that appear
 *   earlier in the input.
 * Inspired by: Cheriyan and Maheshwari (1989): https://doi.org/10.1137/0218072
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

const int STEP = 4;

struct Edge {
	int from, to, cap;
};

int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	auto num_vertices = [&](int k) {
		return 2LL * k + (2LL * k - 1) * STEP + 4;
	};

	auto num_edges = [&](int k) {
		return 1 + 3LL * k + 2LL * k + (2LL * k - 1) * STEP;
	};

	assert(max_n >= num_vertices(1));
	assert(max_m >= num_edges(1));
	assert(max_cap >= 2);

	auto ok = [&](int k) {
		if (num_vertices(k) > max_n) return false;
		if (num_edges(k) > max_m) return false;
		if (2LL * k * k > max_cap) return false;
		return true;
	};

	int k = 1;
	while (ok(k + 1)) k += 1;

	int n = 0;
	std::vector<Edge> edges;

	int s = ++n;

	std::vector<int> a(k), b(k);

	for (int i = 0; i < k; ++i) {
		a[i] = ++n;
		b[i] = ++n;
		edges.push_back({a[i], b[i], (i == k - 1 ? max_cap : 1)});
	}

	int p = ++n;
	int q = ++n;

	edges.push_back({s, p, max_cap});

	for (int i = 0; i < k; ++i) {
		edges.push_back({p, a[i], max_cap});
		edges.push_back({b[i], q, max_cap});
	}

	std::vector<int> line((2 * k - 1) * STEP + 1);

	for (int i = (2 * k - 1) * STEP; i >= 0; --i) {
		line[i] = ++n;
	}

	for (int i = (2 * k - 1) * STEP; i >= 1; --i) {
		edges.push_back({line[i], line[i - 1], max_cap});
	}

	for (int i = 0; i < k; ++i) {
		edges.push_back({q, line[2 * i * STEP], k});
		edges.push_back({p, line[(2 * i + 1) * STEP], k});
	}

	assert(n == num_vertices(k));
	assert((int) edges.size() == num_edges(k));

	std::cout << n << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
