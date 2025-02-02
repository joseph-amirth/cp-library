#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/groups/sum.hpp"
#include "range_query/disjoint_sparse_table.hpp"

namespace groups = algebra::groups;
using range_query::disjoint_sparse_table;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    disjoint_sparse_table<groups::sum<long long>> st(a.begin(), a.end());

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.range_query(l, --r) << '\n';
    }
    return 0;
}
