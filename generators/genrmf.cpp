/*
 * Genrmf graph generator.
 * Source: Goldfarb and Grigoriadis (1988): https://doi.org/10.1007/BF02288321
 * Usage: Input: 2 integers from stdin: max_n, max_m
 *   Output: Graph to stdout in the format:
 *     n m
 *     from_1 to_1 cap_1
 *     ...
 *     from_m to_m cap_m
 *   Vertices are numbered from 1 to n, 1 = source, n = sink.
 */
#include <array>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

struct Edge {
	int from, to, cap;
};

long long num_vertices(int a, int b) {
	return 1LL * a * a * b;
}

long long num_edges(int a, int b) {
	return 4LL * a * (a - 1) * b + 2LL * a * a * (b - 1);
}

long long max_capacity(int a, int c2) {
	return 1LL * a * a * c2;
}

std::array<int, 4> best_parameters(int max_n, int max_m, int max_cap) {
	auto ok = [&](int a, int b, int c2) {
		if (num_vertices(a, b) > max_n) return false;
		if (num_edges(a, b) > max_m) return false;
		if (max_capacity(a, c2) > max_cap) return false;
		return true;
	};

	int c1 = 1;
	int c2 = 1000;

	int a = 2;
	int b = 2;

	while (ok(a + 1, b + 1, c2)) {
		a += 1;
		b += 1;
	}

	while (ok(a + 1, b, c2)) {
		a += 1;
	}

	while (ok(a, b + 1, c2)) {
		b += 1;
	}

	return std::array<int, 4>{a, b, c1, c2};
}
int main() {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	int max_cap = 1'000'000'000;

	assert(max_n >= 8);
	assert(max_m >= 24);
	assert(max_cap >= 4000);

	auto [a, b, c1, c2] = best_parameters(max_n, max_m, max_cap);

	int n = 0;
	std::vector<Edge> edges;

	std::vector<std::vector<std::vector<int>>> v(b,
		std::vector<std::vector<int>>(a, std::vector<int>(a)));

	for (int i = 0; i < b; ++i) {
		for (int j = 0; j < a; ++j) {
			for (int k = 0; k < a; ++k) {
				v[i][j][k] = ++n;
			}
		}
	}

	for (int i = 0; i < b; ++i) {
		for (int j = 0; j < a; ++j) {
			for (int k = 0; k < a; ++k) {
				if (j + 1 < a) {
					edges.push_back({v[i][j][k], v[i][j + 1][k], a * a * c2});
					edges.push_back({v[i][j + 1][k], v[i][j][k], a * a * c2});
				}
				if (k + 1 < a) {
					edges.push_back({v[i][j][k], v[i][j][k + 1], a * a * c2});
					edges.push_back({v[i][j][k + 1], v[i][j][k], a * a * c2});
				}
			}
		}
	}

	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distr(c1, c2);

	std::vector<std::pair<int, int>> perm(a * a);

	for (int j = 0; j < a; ++j) {
		for (int k = 0; k < a; ++k) {
			perm[j * a + k].first = j;
			perm[j * a + k].second = k;
		}
	}

	for (int i = 0; i + 1 < b; ++i) {
		std::shuffle(perm.begin(), perm.end(), rng);
		for (int j = 0; j < a; ++j) {
			for (int k = 0; k < a; ++k) {
				auto [j2, k2] = perm[j * a + k];
				edges.push_back({v[i][j][k], v[i + 1][j2][k2], distr(rng)});
			}
		}
		std::shuffle(perm.begin(), perm.end(), rng);
		for (int j = 0; j < a; ++j) {
			for (int k = 0; k < a; ++k) {
				auto [j2, k2] = perm[j * a + k];
				edges.push_back({v[i + 1][j2][k2], v[i][j][k], distr(rng)});
			}
		}
	}

	assert(n == num_vertices(a, b));
	assert((int) edges.size() == num_edges(a, b));

	std::cout << num_vertices(a, b) << ' ' << edges.size() << '\n';

	for (const Edge& e : edges) {
		std::cout << e.from << ' ' << e.to << ' ' << e.cap << '\n';
	}

	return 0;
}
