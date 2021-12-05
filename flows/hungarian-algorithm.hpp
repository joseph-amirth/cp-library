#pragma once

#include <vector>
#include <limits>
#include <queue>

template<typename T>
std::vector<int> hungarian_algorithm(const std::vector<std::vector<T>> &a) {
    int n = int(a.size());

    T mn_edge_cost = std::numeric_limits<T>::max();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mn_edge_cost = std::min(mn_edge_cost, a[i][j]);
        }
    }

    std::vector<T> ro_r(n, mn_edge_cost), ro_c(n);

    std::vector<int> match_r(n, -1), match_c(n, -1);
    std::vector<bool> visited_c(n);
    std::vector<int> p_c(n);

    auto r_cost = [&](int i, int j) -> T {
        return a[i][j] - ro_r[i] - ro_c[j];
    };

    auto bfs = [&](int s) -> int {
        std::fill(visited_c.begin(), visited_c.end(), false);
        std::queue<int> q;
        q.emplace(s);

        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (int j = 0; j < n; j++) {
                if (r_cost(i, j) == 0 && !visited_c[j]) {
                    p_c[j] = i;
                    if (match_c[j] == -1) {
                        return j;
                    }
                    q.emplace(match_c[j]);
                    visited_c[j] = true;
                }
            }
        }
        return -1;
    };

    for (int u = 0; u < n; u++) {
        while (match_r[u] == -1) {
            int v = bfs(u);
            if (v == -1) {
                T mx_update = std::numeric_limits<T>::max();
                for (int j = 0; j < n; j++) {
                    if (visited_c[j]) {
                        int i = match_c[j];
                        for (int jj = 0; jj < n; jj++) {
                            if (r_cost(i, jj) > 0) {
                                mx_update = std::min(mx_update, r_cost(i, jj));
                            }
                        }
                    }
                }
                for (int j = 0; j < n; j++) {
                    if (r_cost(u, j) > 0) {
                        mx_update = std::min(mx_update, r_cost(u, j));
                    }
                }
                for (int j = 0; j < n; j++) {
                    if (visited_c[j]) {
                        ro_r[match_c[j]] += mx_update;
                        ro_c[j] -= mx_update;
                    }
                }
                ro_r[u] += mx_update;
            } else {
                for (int i = v;;) {
                    if (p_c[i] == u) {
                        match_r[u] = i;
                        match_c[i] = u;
                        break;
                    }
                    match_c[i] = p_c[i];
                    std::swap(match_r[p_c[i]], i);
                }
            }
        }
    }
    return match_r;
}