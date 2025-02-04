#include "directed-graph.hpp"
#include "undirected-graph.hpp"
#include <algorithm>
#include <optional>

namespace graphs {

template <typename Edge>
std::optional<std::vector<int>> eulerian_trail_checked(const directed_graph<Edge> &g) {
    std::vector<bool> visited(g.n);
    std::vector<int> comp(g.m, -1);

    int cur_comp = -1;
    auto dfs = [&](auto &&self, int u) -> void {
        visited[u] = true;
        for (int i : g.adj[u]) {
            int v = g.edges[i].v;
            if (!visited[v]) {
                comp[i] = cur_comp;
                self(self, v);
            }
        }
    };

    for (int u = 0; u < g.n; u++) {
        if (!visited[u]) {
            cur_comp = u;
            dfs(dfs, u);
        }
    }

    bool connected = true;
    for (int i = 1; i < g.m; i++) {
        connected &= comp[i - 1] == comp[i];
    }

    if (!connected) {
        return std::nullopt;
    }

    std::vector<int> in(g.n), out(g.n);
    for (auto &[u, v] : g.edges) {
        out[u] += 1, in[v] += 1;
    }

    // TODO: Implement this.
}

namespace _detail {

template <typename Edge>
std::vector<int> eulerian_trail(const directed_graph<Edge> &g, std::vector<int> in, std::vector<int> out) {
    int start = std::find_if(in.begin(), in.end(), [](int in) { return in > 0; }) - in.begin();

    std::vector<int> st, circuit;
    circuit.reserve(g.m + 1);
    st.push_back(start);

    while (!st.empty()) {
        int x = st.back();
        if (out[x]) {
            int i = g.adj[x][--out[x]];
            st.push_back(g.edges[i].v);
        } else {
            st.pop_back();
            circuit.push_back(x);
        }
    }
    std::reverse(circuit.begin(), circuit.end());
    return circuit;
}

} // namespace _detail

template <typename Edge>
std::vector<int> eulerian_trail(const undirected_graph<Edge> &g) {
    int start;
    for (start = 0; start < g.n; start++) {
        if (!g.adj[start].empty())
            break;
    }

    std::vector<int> deg(g.n), ind(g.n);
    for (int i = 0; i < g.n; i++) {
        deg[i] = ind[i] = g.adj[i].size();
    }

    std::vector<int> st;
    st.push_back(start);

    std::vector<int> circuit;
    circuit.reserve(g.m + 1);

    std::vector<bool> mark(g.m);
    while (!st.empty()) {
        int x = st.back();
        if (!deg[x]) {
            circuit.push_back(x);
            st.pop_back();
        } else {
            while (mark[g.adj[x][--ind[x]]])
                ;
            int i = g.adj[x][ind[x]];
            mark[i] = true;
            i = x ^ g.edges[i].u ^ g.edges[i].v;
            deg[x]--;
            deg[i]--;
            st.push_back(i);
        }
    }
    return circuit;
}

} // namespace graphs
