#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

struct Edge {
	int to, rev, cap;
};

struct Dinic {
	const int n, s, t;
	std::vector<std::vector<Edge>> adj;
	std::vector<int> label, que, iter;

	Dinic(int _n, int _s, int _t) : n(_n), s(_s), t(_t) {
		adj.resize(n);
		label.resize(n);
		que.resize(n);
		iter.resize(n);
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
			std::fill(iter.begin(), iter.end(), 0);
			ans += dfs(t, std::numeric_limits<long long>::max());
		}
		return ans;
	}

	bool bfs() {
		std::fill(label.begin(), label.end(), n);
		label[s] = 0;
		que[0] = s;
		int queBegin = 0;
		int queEnd = 1;
		while (queBegin < queEnd) {
			int v = que[queBegin++];
			for (const Edge& e : adj[v]) {
				if (!e.cap || label[e.to] < n) continue;
				label[e.to] = label[v] + 1;
				if (e.to == t) return true;
				que[queEnd++] = e.to;
			}
		}
		return false;
	}

	long long dfs(int v, long long up) {
		if (v == s) return up;
		long long res = 0;
		for (; iter[v] < (int) adj[v].size(); ++iter[v]) {
			Edge& e = adj[v][iter[v]];
			if (!adj[e.to][e.rev].cap || label[v] <= label[e.to]) continue;
			int d = dfs(e.to, std::min(1LL * adj[e.to][e.rev].cap, up - res));
			e.cap += d;
			adj[e.to][e.rev].cap -= d;
			res += d;
			if (res == up) return res;
		}
		label[v] = n;
		return res;
	}
};

int main() {
	int n, m;
	std::cin >> n >> m;

	Dinic g(n, 0, n - 1);

	for (int i = 0; i < m; ++i) {
		int from, to, cap;
		std::cin >> from >> to >> cap;
		g.addEdge(from - 1, to - 1, cap);
	}

	std::cout << g.flow() << '\n';

	return 0;
}
