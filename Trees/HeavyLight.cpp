#include "LCA.cpp"

template<typename T, typename S>
struct heavy_light {
	std::vector<int> parent, head, depth;
	std::vector<int> tree_pos;
	S st;

	template<typename U, typename F>
	heavy_light(const graph &g, const std::vector<U> &a, T &&e, F &&f) {
		parent.assign(g.n, -1);
		head.assign(g.n, -1);
		depth.assign(g.n, 0);
		tree_pos.assign(g.n, -1);

		std::vector<int> ptr(g.n, -1);
		auto dfs = [&](int u, const auto &self) -> int {
			int tree_sz = 1, mx_sz = 0;
			for (int i : g.adj[u]) {
				int v = g.edges[i].u ^ g.edges[i].v ^ u;
				if (v == parent[u]) {
					continue;
				}
				depth[v] = 1 + depth[u], parent[v] = u;
				int subtree_sz = self(v, self);
				tree_sz += subtree_sz;
				if (subtree_sz > mx_sz) {
					ptr[u] = v;
					mx_sz = subtree_sz;
				}
			}
			return tree_sz;
		};

		dfs(0, dfs);

		std::vector<T> values(g.n);
		for (int u = 0, nxt = 0; u < g.n; u++) {
			if (parent[u] == -1 || ptr[parent[u]] != u) {
				for (int v = u; v != -1; v = ptr[v]) {
					tree_pos[v] = nxt++;
					values[tree_pos[v]] = T(a[v]);
					head[v] = u;
				}
			}
		}
		st = S(values, g.n, e, f);
	}

	template <typename F>
	void visit_path(int u, int v, F &&f) {
		for (; head[u] != head[v]; u = parent[head[u]]) {
			if (depth[head[u]] < depth[head[v]]) {
				std::swap(u, v);
			}
			f(tree_pos[head[u]], tree_pos[u]);
		}
		if (depth[u] < depth[v]) {
			std::swap(u, v);
		}
		f(tree_pos[v], tree_pos[u]);
	}

	T query(int u, int v) {
		T ans = st.e;
		visit_path(u, v, [&](int l, int r) {
			ans = st.f(ans, st.query(l, r));
		});
		return ans;
	}

	template <typename U>
	void update(int u, const U &val) {
		st.update(tree_pos[u], val);
	}

	template <typename U>
	void update(int u, int v, const U &val) {
		visit_path(u, v, [&](int l, int r) {
			st.update(l, r, val);
		});
	}
};