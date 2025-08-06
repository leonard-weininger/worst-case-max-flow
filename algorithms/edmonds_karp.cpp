#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

struct Edge {
	int to, rev, cap;
};

struct EdmondsKarp {
	const int n, s, t;
	std::vector<std::vector<Edge>> adj;
	std::vector<int> que, parent, path_cap;

	EdmondsKarp(int _n, int _s, int _t) : n(_n), s(_s), t(_t) {
		adj.resize(n);
		que.resize(n);
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
		while (bfs()) {
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

	bool bfs() {
		std::fill(parent.begin(), parent.end(), -1);
		parent[s] = -2;
		path_cap[s] = std::numeric_limits<int>::max();
		que[0] = s;
		int queBegin = 0;
		int queEnd = 1;
		while (queBegin < queEnd) {
			int v = que[queBegin++];
			for (const Edge& e : adj[v]) {
				if (!e.cap || parent[e.to] != -1) continue;
				parent[e.to] = e.rev;
				path_cap[e.to] = std::min(path_cap[v], e.cap);
				if (e.to == t) return true;
				que[queEnd++] = e.to;
			}
		}
		return false;
	}
};

int main() {
	int n, m;
	std::cin >> n >> m;

	EdmondsKarp g(n, 0, n - 1);

	for (int i = 0; i < m; ++i) {
		int from, to, cap;
		std::cin >> from >> to >> cap;
		g.addEdge(from - 1, to - 1, cap);
	}

	std::cout << g.flow() << '\n';

	return 0;
}
