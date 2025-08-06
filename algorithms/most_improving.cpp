#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
 
struct Edge {
	int to, rev, cap;
};

struct MostImprovingAugmentingPaths {
	const int n, s, t;
	std::vector<std::vector<Edge>> adj;
	std::vector<int> parent, path_cap;

	MostImprovingAugmentingPaths(int _n, int _s, int _t) : n(_n), s(_s), t(_t) {
		adj.resize(n);
		parent.resize(n);
		path_cap.resize(n);
	}

	void addEdge(int from, int to, int cap) {
		if (from != to && cap >= 1) {
			adj[from].push_back({to, (int) adj[to].size(), cap});
			adj[to].push_back({from, (int) adj[from].size() - 1, 0});
		}
	}

	long long flow() {
		long long ans = 0;
		while (dijkstra()) {
			ans += path_cap[t];
			int v = t;
			while (v != s) {
				Edge& e = adj[v][parent[v]];
				e.cap += path_cap[t];
				adj[e.to][e.rev].cap -= path_cap[t];
				v = e.to;
			}
		}
		return ans;
	}

	bool dijkstra() {
		std::fill(path_cap.begin(), path_cap.end(), 0);
		parent[s] = -2;
		path_cap[s] = std::numeric_limits<int>::max();
		std::priority_queue<std::pair<int, int>> pq;
		pq.emplace(path_cap[s], s);
		while (!pq.empty()) {
			auto [flow, v] = pq.top();
			pq.pop();
			if (v == t) return true;
			if (flow < path_cap[v]) continue;
			for (const Edge& e : adj[v]) {
				if (std::min(flow, e.cap) > path_cap[e.to]) {
					path_cap[e.to] = std::min(flow, e.cap);
					parent[e.to] = e.rev;
					pq.emplace(path_cap[e.to], e.to);
				}
			}
		}
		return false;
	}
};
 
int main() {
	int n, m;
	std::cin >> n >> m;

	MostImprovingAugmentingPaths g(n, 0, n - 1);

	for (int i = 0; i < m; ++i) {
		int from, to, cap;
		std::cin >> from >> to >> cap;
		g.addEdge(from - 1, to - 1, cap);
	}
 
	std::cout << g.flow() << '\n';
 
	return 0;
}
