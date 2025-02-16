#pragma once

#include <cassert>
#include <concepts>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

namespace networks {

// https://cp-algorithms.com/graph/hungarian-algorithm.html#implementation-of-the-hungarian-algorithm.
template <std::integral T>
std::pair<T, std::vector<int>> hungarian_algorithm(const std::vector<std::vector<T>> &a) {
    constexpr auto inf = std::numeric_limits<T>::max();

    int m = (int)a.size(), n = (int)a.front().size();
    assert(m <= n);

    std::vector<T> u(m), v(n), min_deltas(n);
    std::vector<int> match(n, -1), prev(n);
    std::vector<bool> visited(n);

    for (int i = 0; i < m; i++) {
        int cur_i = i;
        int cur_j = -1;

        min_deltas.assign(n, inf);
        prev.assign(n, -1);
        visited.assign(n, false);

        do {
            if (cur_j != -1)
                visited[cur_j] = true;
            int next_j = -1;
            T min_delta = inf;
            for (int j = 0; j < n; j++)
                if (!visited[j]) {
                    T delta = a[cur_i][j] - u[cur_i] - v[j];
                    if (delta < min_deltas[j])
                        min_deltas[j] = delta, prev[j] = cur_j;
                    if (min_deltas[j] < min_delta)
                        min_delta = min_deltas[j], next_j = j;
                }
            u[i] += min_delta;
            for (int j = 0; j < n; j++)
                if (visited[j]) {
                    u[match[j]] += min_delta, v[j] -= min_delta;
                } else {
                    min_deltas[j] -= min_delta;
                }
            cur_j = next_j;
            cur_i = match[next_j];
        } while (cur_i != -1);

        do {
            int prev_j = prev[cur_j];
            int cur_i = (prev_j == -1 ? i : match[prev_j]);
            match[cur_j] = cur_i;
            cur_j = prev[cur_j];
        } while (cur_j != -1);
    }

    T cost = 0;
    cost += std::accumulate(u.begin(), u.end(), (T)0);
    cost += std::accumulate(v.begin(), v.end(), (T)0);

    std::vector<int> ans(m);
    for (int j = 0; j < n; j++)
        if (match[j] != -1) {
            ans[match[j]] = j;
        }

    return std::make_pair(cost, ans);
}

} // namespace networks
