#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

struct Edge {
	int to, rev, cap;
};

struct FordFulkersonDFS {
	const int n, s, t;
	std::vector<std::vector<Edge>> adj;
	std::vector<bool> visited;

	FordFulkersonDFS(int _n, int _s, int _t) : n(_n), s(_s), t(_t) {
		adj.resize(n);
		visited.resize(n);
	}

	void addEdge(int from, int to, int cap) {
		if (from != to && cap >= 1) {
			adj[from].push_back({to, (int) adj[to].size(), cap});
			adj[to].push_back({from, (int) adj[from].size() - 1, 0});
		}
	}

	int dfs(int v, int flow) {
		if (v == t) return flow;
		visited[v] = true;
		for (Edge& e : adj[v]) {
			if (!visited[e.to] && e.cap) {
				int d = dfs(e.to, std::min(flow, e.cap));
				if (d >= 1) {
					e.cap -= d;
					adj[e.to][e.rev].cap += d;
					return d;
				}
			}
		}
		return 0;
	}

	long long flow() {
		long long flow = 0;
		while (true) {
			std::fill(visited.begin(), visited.end(), false);
			int d = dfs(s, std::numeric_limits<int>::max());
			if (d == 0) break;
			flow += d;
		}
		return flow;
	}
};

int main() {
	int n, m;
	std::cin >> n >> m;

	 FordFulkersonDFS g(n, 0, n - 1);

	for (int i = 0; i < m; ++i) {
		int from, to, cap;
		std::cin >> from >> to >> cap;
		g.addEdge(from - 1, to - 1, cap);
	}

	std::cout << g.flow() << '\n';

	return 0;
}
