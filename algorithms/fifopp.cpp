#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct Edge {
	int to, rev, cap;
};

struct FIFOPP {
	const int n, s, t;
	std::vector<std::vector<Edge>> adj;
	std::vector<int> height, iter;
	std::vector<long long> excess;
	std::queue<int> active;

	FIFOPP(int _n, int _s, int _t) : n(_n), s(_s), t(_t),
		adj(n), height(n), iter(n), excess(n) {}

	void add_edge(int from, int to, int cap) {
		if (from != to && from != t && to != s && cap >= 1) {
			adj[from].push_back({to, (int) adj[to].size(), cap});
			adj[to].push_back({from, (int) adj[from].size() - 1, 0});
		}
	}

	long long flow() {
		preprocess();
		while (!active.empty()) {
			int v = active.front();
			active.pop();
			discharge(v);
		}
		return excess[t];
	}

	void push(int v, Edge& e) {
		int d = std::min(excess[v], 1LL * e.cap);
		if (d <= 0 || height[v] <= height[e.to]) return;
		if (excess[e.to] == 0 && e.to != s && e.to != t) {
			active.push(e.to);
		}
		excess[v] -= d;
		excess[e.to] += d;
		e.cap -= d;
		adj[e.to][e.rev].cap += d;
	}

	void relabel(int v) {
		height[v] = 2 * n;
		for (const Edge& e : adj[v]) {
			if (e.cap > 0) {
				height[v] = std::min(height[v], height[e.to] + 1);
			}
		}
	}

	void discharge(int v) {
		while (excess[v] > 0) {
			while (iter[v] < (int) adj[v].size()) {
				Edge& e = adj[v][iter[v]];
				push(v, e);
				if (excess[v] <= 0) return;
				iter[v] += 1;
			}
			relabel(v);
			iter[v] = 0;
		}
	}

	void preprocess() {
		std::fill(height.begin(), height.end(), n);
		height[t] = 0;
		std::queue<int> q;
		q.push(t);
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			for (const Edge& e : adj[v]) {
				if (height[e.to] == n && adj[e.to][e.rev].cap > 0) {
					height[e.to] = height[v] + 1;
					q.push(e.to);
				}
			}
		}
		height[s] = n;
		excess[s] = (1LL << 60) + 5;
		for (Edge& e : adj[s]) {
			push(s, e);
		}
	}
};

int main() {
	int n, m;
	std::cin >> n >> m;

	FIFOPP g(n, 0, n - 1);

	for (int i = 0; i < m; ++i) {
		int u, v, w;
		std::cin >> u >> v >> w;
		g.add_edge(u - 1, v - 1, w);
	}

	std::cout << g.flow() << '\n';

	return 0;
}
