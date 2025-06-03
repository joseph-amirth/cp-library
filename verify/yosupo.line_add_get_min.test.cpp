#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"

#include <bits/stdc++.h>
using namespace std;

#include "geometry/li_chao_tree/line_int.hpp"

using ll = long long;
using geometry::li_chao_tree;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 1'000'000'000;
    li_chao_tree<int, long long> lct(-N, N);

    int n, q;
    cin >> n >> q;

    for (int i = 0; i < n; i++) {
        long long a, b;
        cin >> a >> b;
        lct.emplace_function(a, b);
    }

    while (q--) {
        int type;
        cin >> type;

        if (type == 0) {
            long long a, b;
            cin >> a >> b;
            lct.emplace_function(a, b);
        } else {
            int p;
            cin >> p;
            cout << lct.get_minimum(p) << '\n';
        }
    }
    return 0;
}
