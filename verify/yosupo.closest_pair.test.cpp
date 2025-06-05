#define PROBLEM "https://judge.yosupo.jp/problem/closest_pair"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "geometry/closest_pair_of_points.hpp"
#include "geometry/point.hpp"

using geometry::closest_pair_of_points;
using geometry::point;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<point<int>> points(n);
        for (auto &[x, y] : points) {
            cin >> x >> y;
        }

        auto [i, j] = closest_pair_of_points(points);
        cout << i << ' ' << j << '\n';
    }
    return 0;
}
