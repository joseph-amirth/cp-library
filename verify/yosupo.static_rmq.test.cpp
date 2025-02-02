#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/semigroups/extremum.hpp"
#include "range_query/sparse_table.hpp"

namespace semigroups = algebra::semigroups;
using range_query::sparse_table;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    sparse_table<semigroups::extremum<int>> st(a.begin(), a.end());

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.range_query(l, --r) << '\n';
    }
    return 0;
}
