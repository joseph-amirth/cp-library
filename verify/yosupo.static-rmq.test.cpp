#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <bits/stdc++.h>
using namespace std;

#include "data-structures/sparse-table/sparse-table.hpp"
#include "algebra/groupoid/extremum.hpp"

using data_structures::sparse_table;
using algebra::extremum_semigroup;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a) cin >> x;

    sparse_table<extremum_semigroup<int>> st(a.begin(), a.end());

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.range_query(l, --r) << '\n';
    }
    return 0;
}
