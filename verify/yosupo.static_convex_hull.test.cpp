#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "geometry/convex_hull.hpp"
#include "geometry/point.hpp"

using geometry::convex_hull;
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

        auto ans = convex_hull(points);

        cout << ans.size() << '\n';
        for (int i : ans) {
            cout << points[i].x << ' ' << points[i].y << '\n';
        }
    }
    return 0;
}
