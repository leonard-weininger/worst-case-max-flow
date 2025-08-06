#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

struct Edge {
	int to, cap;
};

class FordFulkersonRandomDFS {
private:
	const int n, s, t;
	std::vector<std::vector<int>> adj;
	std::vector<Edge> edges;
	std::vector<bool> visited;

public:
	FordFulkersonRandomDFS(int _n, int _s, int _t) : n(_n), s(_s), t(_t) {
		adj.resize(n);
		visited.resize(n);
	}

	void addEdge(int from, int to, int cap) {
		if (from != to && cap >= 1) {
			adj[from].push_back(edges.size());
			adj[to].push_back(edges.size() + 1);
			edges.push_back({to, cap});
			edges.push_back({from, 0});
		}
	}

	int dfs(int v, int flow) {
		if (v == t) return flow;
		visited[v] = true;
		for (int eid : adj[v]) {
			Edge& e = edges[eid];
			if (!visited[e.to] && e.cap >= 1) {
				int d = dfs(e.to, std::min(flow, e.cap));
				if (d >= 1) {
					e.cap -= d;
					edges[eid ^ 1].cap += d;
					return d;
				}
			}
		}
		return 0;
	}

	long long flow() {
		long long ans = 0;
		std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
		for (std::vector<int>& vec : adj) {
			std::shuffle(vec.begin(), vec.end(), rng);
		}
		while (true) {
			std::fill(visited.begin(), visited.end(), false);
			int d = dfs(s, std::numeric_limits<int>::max());
			if (d == 0) break;
			ans += d;
		}
		return ans;
	}
};

int main() {
	int n, m;
	std::cin >> n >> m;

	FordFulkersonRandomDFS g(n, 0, n - 1);

	for (int i = 0; i < m; ++i) {
		int from, to, cap;
		std::cin >> from >> to >> cap;
		g.addEdge(from - 1, to - 1, cap);
	}

	std::cout << g.flow() << '\n';

	return 0;
}
