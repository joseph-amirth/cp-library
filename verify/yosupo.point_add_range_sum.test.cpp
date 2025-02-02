#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/groups/sum.hpp"
#include "range_query/fenwick_tree.hpp"

namespace groups = algebra::groups;
using range_query::fenwick_tree;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    fenwick_tree<groups::sum<long long>> ft(a.begin(), a.end());

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
