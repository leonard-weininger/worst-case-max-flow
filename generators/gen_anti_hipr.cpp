/*
 * Anti hi_pr.c Graph Generator
 * Author: Leonard Weininger
 * Date: 2025-08-05
 * License: CC0
 * Description: Generates a flow graph, that is specifically 
 *   designed, such that the famous hi_pr.c implementation
 *   (see for example: https://github.com/carlhub/hipr),
 *   requires $\Omega(n^2 \sqrt(m))$ time to find the max flow.
 * Inspired by: Cheriyan and Maheshwari (1989): https://doi.org/10.1137/0218072
 * Usage: Input: 1 integer from stdin: max_num_edges
 *   Output: Graph to stdout in DIMACS format:
 *     p max [num_vertices] [num_edges]
 *     n [source_vertex] s
 *     n [sink_vertex] t
 *     a [from_1] [to_1] [cap_1]
 *     ...
 *     a [from_m] [to_m] [cap_m]
 */
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

struct Edge {
	int from, to, cap;
};

long long num_edges(int K, int W, int L) {
	return 2LL * (18LL * L + 2LL * W * (2LL * K + 5) + 1LL * W * W + 5LL);
}

long long max_capacity(int K, int W, int L) {
	return 2LL * L * W * W + 4LL * L + 1LL;
}

long long estimated_work(int K, int W, int L) {
	return 2LL * L * W * W * K;
}

std::array<int, 3> best_parameters(int max_m, int max_cap) {
	auto ok = [&](int K, int W, int L) {
		if (num_edges(K, W, L) > max_m) return false;
		if (max_capacity(K, W, L) > max_cap) return false;
		return true;
	};

	int best_K = 1;
	int best_W = 1;
	int best_L = 1;

	for (int K = 1; num_edges(K, 1, 1) <= max_m; ++K) {
		if (num_edges(K, 1, 1) > max_m) break;
		if (max_capacity(K, 1, 1) > max_cap) break;
		for (int W = 1, L = 1; num_edges(K, W, 1) <= max_m; ++W) {
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
	int max_m;
	std::cin >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_m >= 74);
	assert(max_cap >= 4);

	auto [K, W, L] = best_parameters(max_m, max_cap);
	int inf = max_capacity(K, W, L);

	int n = 0;
	std::vector<Edge> edges;

	int s2 = ++n;
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

	int t = ++n;

	edges.push_back({s2, s, inf});
	edges.push_back({s, t, 1});
	edges.push_back({line[0], t, inf});

	for (int i = 0; i < 2 * L; ++i) {
		edges.push_back({s, c[i], W * W + 2});
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

	std::sort(edges.begin(), edges.end(), [&](const Edge& e1, const Edge& e2) {
		if (e1.from < e2.from) return true;
		if (e1.from > e2.from) return false;
		return e1.to < e2.to;
	});

	// Rest is just making sure the edge order by hi_pr.c will be correct
	std::vector<std::vector<int>> ve(n + 1);

	for (int i = 0; i < (int) edges.size(); ++i) {
		int u = edges[i].from;
		int v = edges[i].to;
		ve[u].push_back(i);
		ve[v].push_back(i);
	}

	std::vector<int> order;

	for (int i = 1; i <= n; ++i) {
		for (int id : ve[i]) order.push_back(id);
	}
	
	assert(order.size() % 2 == 0);

	std::vector<int> have(edges.size(), -1);
	std::vector<int> open;

	std::vector<Edge> final_edges;

	for (int i = 0; i < (int) order.size(); ++i) {
		int u = -1;
		if (have[order[i]] == -1) {
			if (open.empty()) {
				open.push_back((int) ve.size());
				ve.push_back(std::vector<int>());
			}
			u = open.back();
			open.pop_back();
			have[order[i]] = u;
			ve[u].push_back(order[i]);
		}
		else {
			u = have[order[i]];
			open.push_back(u);
		}
		i += 1;
		int v = -1;
		if (have[order[i]] == -1) {
			if (open.empty()) {
				open.push_back((int) ve.size());
				ve.push_back(std::vector<int>());
			}
			v = open.back();
			open.pop_back();
			have[order[i]] = v;
			ve[v].push_back(order[i]);
		}
		else {
			v = have[order[i]];
			open.push_back(v);
		}
		final_edges.push_back({u, v, 1});
	}

	for (int i = n + 1; i < (int) ve.size(); ++i) {
		for (int id : ve[i]) {
			final_edges.push_back(edges[id]);
		}
	}

	assert((int) ve.size() <= max_m);
	assert((int) final_edges.size() <= max_m);

	std::cout << "p max " << ve.size() - 1 << ' ' << final_edges.size() << '\n';
	std::cout << "n 1 s" << '\n';
	std::cout << "n " << t << " t" << '\n';

	for (const Edge& e : final_edges) {
		std::cout << "a " << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
