/*
 * Anti-Highest-Label-Preflow-Push Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-07-27
 * License: CC0
 * Description: Generates a flow graph on which the HLPP algorithm
 *   requires \Omega(n^2 * sqrt(m)) time to compute the maximum flow.
 *   Only works if HLPP prioritizes edges, that appear
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
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

struct Edge {
	int from, to, cap;
};

long long num_vertices(int K, int W, int L) {
	return 10LL + 8LL * L + 2LL * W * (2LL * K + 3);
}

long long num_edges(int K, int W, int L) {
	return 18LL * L + 2LL * W * (2LL * K + 5) + 1LL * W * W + 2LL;
}

long long max_capacity(int K, int W, int L) {
	return 2LL * L * W * W + 4LL * L;
}

long long estimated_work(int K, int W, int L) {
	return 2LL * L * W * W * K;
}

std::array<int, 3> best_parameters(int max_n, int max_m, int max_cap) {
	auto ok = [&](int K, int W, int L) {
		if (num_vertices(K, W, L) > max_n) return false;
		if (num_edges(K, W, L) > max_m) return false;
		if (max_capacity(K, W, L) > max_cap) return false;
		return true;
	};

	int best_K = 1;
	int best_W = 1;
	int best_L = 1;

	for (int K = 1; num_vertices(K, 1, 1) <= max_n; ++K) {
		if (num_edges(K, 1, 1) > max_m) break;
		if (max_capacity(K, 1, 1) > max_cap) break;
		for (int W = 1, L = 1; num_vertices(K, W, 1) <= max_n; ++W) {
			while (ok(K, W, L)) L += 1;
			while (L >= 1 && !ok(K, W, L)) L -= 1;
			if (L == 0) break;
			if (estimated_work(K, W, L) > estimated_work(best_K, best_W, best_L)) {
				best_K = K;
				best_W = W;
				best_L = L;
			}
		}
	}

	return std::array<int, 3>{best_K, best_W, best_L};
}

int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 30);
	assert(max_m >= 37);
	assert(max_cap >= 4);

	auto [K, W, L] = best_parameters(max_n, max_m, max_cap);
	int inf = max_capacity(K, W, L);

	int n = 0;
	std::vector<Edge> edges;

	int s = ++n;
	int a1 = ++n;

	std::vector<std::vector<int>> v1(K + 3, std::vector<int>(W));

	for (int i = 0; i < K + 3; ++i) {
		for (int j = 0; j < W; ++j) {
			v1[i][j] = ++n;
		}
	}

	int b1 = ++n;
	int a2 = ++n;

	std::vector<std::vector<int>> v2(K + 3, std::vector<int>(W));

	for (int i = 0; i < K + 3; ++i) {
		for (int j = 0; j < W; ++j) {
			v2[i][j] = ++n;
		}
	}

	int b2 = ++n;
	int p1 = ++n;

	std::vector<std::vector<int>> v3(K, std::vector<int>(W));

	for (int i = 0; i < K; ++i) {
		for (int j = 0; j < W; ++j) {
			v3[i][j] = ++n;
		}
	}

	int q1 = ++n;
	int p2 = ++n;

	std::vector<std::vector<int>> v4(K, std::vector<int>(W));

	for (int i = 0; i < K; ++i) {
		for (int j = 0; j < W; ++j) {
			v4[i][j] = ++n;
		}
	}

	int q2 = ++n;

	std::vector<int> c(2 * L);

	for (int i = 0; i < 2 * L; ++i) {
		c[i] = ++n;
	}

	std::vector<int> line(6 * L + 1);

	for (int i = 6 * L; i >= 0; --i) {
		line[i] = ++n;
	}

	for (int i = 0; i < 2 * L; ++i) {
		edges.push_back({s, c[i], inf});
		edges.push_back({c[i], line[0], 1});
		edges.push_back({c[i], (i % 2 == 0 ? a2 : a1), W * W});
		edges.push_back({c[i], (i % 2 == 0 ? b2 : b1), 1});
	}

	for (int j = 0; j < W; ++j) {
		edges.push_back({a1, v1[0][j], W});
		edges.push_back({a2, v2[0][j], W});
	}

	for (int i = 0; i + 1 < K + 3; ++i) {
		for (int j = 0; j < W; ++j) {
			edges.push_back({v1[i][j], v1[i + 1][j], W});
			edges.push_back({v2[i][j], v2[i + 1][j], W});
		}
	}

	for (int j = 0; j < W; ++j) {
		edges.push_back({v1[K + 2][j], b1, W});
		edges.push_back({v2[K + 2][j], b2, W});
	}

	for (int i = 0; i < L; ++i) {
		edges.push_back({b1, line[6 * i], 1});
		edges.push_back({b2, line[6 * i + 3], 1});
	}

	edges.push_back({a1, p1, inf});
	edges.push_back({a2, p2, inf});

	for (int j = 0; j < W; ++j) {
		edges.push_back({p1, v3[0][j], inf});
		edges.push_back({p2, v4[0][j], inf});
	}

	for (int i = 0; i + 1 < K; ++i) {
		for (int j = 0; j < W; ++j) {
			edges.push_back({v3[i][j], v3[i + 1][j], inf});
			edges.push_back({v4[i][j], v4[i + 1][j], inf});
		}
	}

	for (int j = 0; j < W; ++j) {
		edges.push_back({v3[K - 1][j], q1, inf});
		edges.push_back({v4[K - 1][j], q2, inf});
	}

	for (int i = 0; i < W; ++i) {
		for (int j = 0; j < W; ++j) {
			edges.push_back({v3[0][i], v4[0][j], 1});
		}
	}

	for (int i = 0; i < L; ++i) {
		edges.push_back({q1, line[6 + 6 * i], W * W});
		edges.push_back({q2, line[3 + 6 * i], W * W});
	}

	for (int i = 6 * L; i >= 1; --i) {
		edges.push_back({line[i], line[i - 1], inf});
	}

	assert(n == num_vertices(K, W, L));
	assert((int) edges.size() == num_edges(K, W, L));

	std::cout << n << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
