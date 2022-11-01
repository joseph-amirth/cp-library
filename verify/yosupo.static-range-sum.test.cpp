#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "data-structures/sparse-table/disjoint-sparse-table.hpp"
#include "algebra/groupoid/common.hpp"

using data_structures::disjoint_sparse_table;
using algebra::sum_monoid;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a) cin >> x;

    disjoint_sparse_table<sum_monoid<long long>> st(a);

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.range_query(l, --r) << '\n';
    }
    return 0;
}
