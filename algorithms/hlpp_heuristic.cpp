#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
	int to, rev, cap;
};

// Highest-Label Preflow Push with Global Relabeling and Gap Heuristics
struct HLPP {
	const int n, s, t;
	std::vector<std::vector<Edge>> adj;
	std::vector<int> height, que, count, iter;
	std::vector<long long> excess;
	std::vector<std::vector<int>> buckets;
	int max_height, work;

	HLPP(int _n, int _s, int _t) : n(_n), s(_s), t(_t), adj(n),
		height(n), que(n), count(n), iter(n), excess(n), buckets(n) {}

	void addEdge(int from, int to, int cap) {
		if (from != to && from != t && to != s && cap >= 1) {
			adj[from].push_back({to, (int) adj[to].size(), cap});
			adj[to].push_back({from, (int) adj[from].size() - 1, 0});
		}
	}

	long long flow() {
		std::fill(excess.begin(), excess.end(), 0);
		excess[s] = (1LL << 60) + 5;
		globalRelabel();
		for (Edge& e : adj[s]) {
			if (height[e.to] < n) {
				push(s, e);
				max_height = std::max(max_height, height[e.to]);
			}
		}
		for (; max_height > 0; --max_height) {
			while (!buckets[max_height].empty()) {
				int v = buckets[max_height].back();
				buckets[max_height].pop_back();
				discharge(v);
				if (work > 5 * n) globalRelabel();
			}
		}
		return excess[t];
	}

	void globalRelabel() {
		std::fill(height.begin(), height.end(), n);
		std::fill(count.begin(), count.end(), 0);
		std::fill(iter.begin(), iter.end(), 0);
		for (int i = 0; i < n; ++i) {
			buckets[i].clear();
		}
		int queBegin = 0;
		int queEnd = 0;
		height[t] = 0;
		max_height = 0;
		work = 0;
		que[queEnd++] = t;
		while (queBegin < queEnd) {
			int v = que[queBegin++];
			if (height[v] < n && v != s && v != t) {
				count[height[v]] += 1;
				if (excess[v] > 0) {
					buckets[height[v]].push_back(v);
					max_height = height[v];
				}
			}
			for (const Edge& e : adj[v]) {
				if (!adj[e.to][e.rev].cap || height[e.to] < n) continue;
				height[e.to] = height[v] + 1;
				que[queEnd++] = e.to;
			}
		}
		height[s] = n;
	}

	void push(int v, Edge& e) {
		if (!excess[e.to]) buckets[height[e.to]].push_back(e.to);
		int d = std::min(excess[v], 1LL * e.cap);
		e.cap -= d;
		adj[e.to][e.rev].cap += d;
		excess[v] -= d;
		excess[e.to] += d;
	}

	void relabel(int v) {
		work += 1;
		count[height[v]] -= 1;
		height[v] = n;
		for (const Edge& e : adj[v]) {
			if (e.cap > 0) {
				height[v] = std::min(height[v], height[e.to] + 1);
			}
		}
		if (height[v] < n) {
			count[height[v]] += 1;
			if (excess[v] > 0) {
				buckets[height[v]].push_back(v);
				max_height = height[v];
			}
		}
	}

	void discharge(int v) {
		while (iter[v] < (int) adj[v].size()) {
			Edge& e = adj[v][iter[v]];
			if (e.cap > 0 && height[v] > height[e.to]) {
				push(v, e);
				if (excess[v] == 0) return;
			}
			iter[v] += 1;
		}
		if (count[height[v]] > 1) {
			relabel(v);
			iter[v] = 0;
		}
		else {
			int gap = height[v];
			for (int i = 0; i < n; ++i) {
				if (gap <= height[i] && height[i] < n) {
					count[height[i]] = 0;
					height[i] = n;
				}
			}
		}
	}
};

int main() {
	int n, m;
	std::cin >> n >> m;

	HLPP g(n, 0, n - 1);

	for (int i = 0; i < m; ++i) {
		int u, v, w;
		std::cin >> u >> v >> w;
		g.addEdge(u - 1, v - 1, w);
	}

	std::cout << g.flow() << '\n';

	return 0;
}
