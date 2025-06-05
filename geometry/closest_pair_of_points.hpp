#pragma once

#include "misc/type_promotion.hpp"
#include "point.hpp"

#include <algorithm>
#include <assert.h>
#include <numeric>
#include <vector>

namespace geometry {

template <typename T>
std::pair<int, int> closest_pair_of_points(const std::vector<point<T>> &p) {
    using D = promote_t<T>;

    int n = int(p.size());
    assert(n > 1);

    std::vector<int> ind(n), temp(n);
    std::iota(ind.begin(), ind.end(), 0);
    std::sort(ind.begin(), ind.end(), [&](int i, int j) {
        return p[i] < p[j];
    });

    auto sq = [](T x) -> D {
        return (D)x * x;
    };

    std::pair<int, int> best_pair(-1, -1);
    D best_distance = std::numeric_limits<D>::max();

    auto update_dist = [&](int i, int j) -> void {
        D cur_dist = sq(p[i].x - p[j].x) + sq(p[i].y - p[j].y);
        if (cur_dist < best_distance) {
            best_pair = std::make_pair(i, j);
            best_distance = cur_dist;
        }
    };

    auto recurse = [&](auto &&self, int l, int r) -> void {
        if (r - l <= 3) {
            for (int i = l; i < r; i++) {
                for (int j = i + 1; j < r; j++) {
                    update_dist(ind[i], ind[j]);
                }
            }
            std::sort(ind.begin() + l, ind.begin() + r, [&](int i, int j) {
                return p[i].y < p[j].y;
            });
            return;
        }

        int mid = (l + r) / 2;
        T mid_x = p[ind[mid]].x;
        self(self, l, mid);
        self(self, mid, r);

        std::merge(ind.begin() + l, ind.begin() + mid, ind.begin() + mid, ind.begin() + r, temp.begin(),
                   [&](int i, int j) {
                       return p[i].y < p[j].y;
                   });

        std::copy(temp.begin(), temp.begin() + r - l, ind.begin() + l);

        for (int i = l, k = 0; i < r; i++) {
            if (sq(p[ind[i]].x - mid_x) < best_distance) {
                for (int j = k - 1; j >= 0 && sq(p[ind[i]].y - p[temp[j]].y) < best_distance; j--) {
                    update_dist(ind[i], temp[j]);
                }
                temp[k++] = ind[i];
            }
        }
    };

    recurse(recurse, 0, n);
    return best_pair;
}

} // namespace geometry
