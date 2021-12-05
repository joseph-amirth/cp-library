#pragma once

#include "point.hpp"
#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>

long long signed_triangle_area(const point<long long> &a, const point<long long> &b, const point<long long> &c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

template<typename T>
std::vector<int> convex_hull(const std::vector<point<T>> &p) {
    int n = int(p.size());
    assert(n > 0);

    std::vector<int> ind(n);
    std::iota(ind.begin(), ind.end(), 0);
    sort(ind.begin(), ind.end(), [&](int i, int j) {
        return p[i].x < p[j].x || (p[i].x == p[j].x && p[i].y < p[j].y);
    });

    int a = ind[0], b = ind[n - 1];
    int c = std::count_if(ind.begin(), ind.end(), [&](int i) {
        return signed_triangle_area(p[i], p[a], p[b]) == 0;
    });

    if (c == n) {
        if (p[a] == p[b]) {
            return {a};
        }
        return {a, b};
    }

    std::vector<int> upper_hull = {a}, lower_hull = {a};
    for (int i = 0; i < n; i++) {
        if (signed_triangle_area(p[ind[i]], p[a], p[b]) > 0 || ind[i] == b) {
            while (upper_hull.size() > 1) {
                int j = upper_hull[(int) upper_hull.size() - 2], k = upper_hull[(int) upper_hull.size() - 1];
                if (signed_triangle_area(p[ind[i]], p[j], p[k]) >= 0) {
                    upper_hull.pop_back();
                } else {
                    break;
                }
            }
            upper_hull.push_back(ind[i]);
        }

        if (signed_triangle_area(p[ind[i]], p[a], p[b]) < 0 || ind[i] == b) {
            while (lower_hull.size() > 1) {
                int j = lower_hull[(int) lower_hull.size() - 2], k = lower_hull[(int) lower_hull.size() - 1];
                if (signed_triangle_area(p[ind[i]], p[j], p[k]) <= 0) {
                    lower_hull.pop_back();
                } else {
                    break;
                }
            }
            lower_hull.push_back(ind[i]);
        }
    }

    std::vector<int> final_hull(std::move(lower_hull));
    if (upper_hull.size() > 2) {
        final_hull.insert(final_hull.end(), upper_hull.rbegin() + 1, upper_hull.rend() - 1);
    }
    return final_hull;
}