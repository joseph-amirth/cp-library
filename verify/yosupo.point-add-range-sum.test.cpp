#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "data-structures/fenwick-tree.hpp"
#include "algebra/groupoid/common-groups.hpp"

using data_structures::fenwick_tree;
using algebra::sum_group;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a) cin >> x;

    fenwick_tree<sum_group<long long>> ft(n);
    for (int i = 0; i < n; i++) {
        ft.apply(i, a[i]);
    }

    while (q--) {
        int type;
        cin >> type;

        if (type == 0) {
            int p, x;
            cin >> p >> x;
            ft.apply(p, x);
        } else {
            int l, r;
            cin >> l >> r, --r;
            cout << ft.range_query(l, r) << '\n';
        }
    }
    return 0;
}
