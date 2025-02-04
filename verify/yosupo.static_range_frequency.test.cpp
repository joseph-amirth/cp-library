#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

#include <bits/stdc++.h>
using namespace std;

#include "range_query/segment_tree/merge_sort_tree.hpp"

using range_query::merge_sort_tree;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    if (n == 0) {
        for (int i = 0; i < q; i++) {
            cout << 0 << '\n';
        }
        return 0;
    }

    vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    merge_sort_tree<int> mst(a.begin(), a.end());

    while (q--) {
        int l, r, x;
        cin >> l >> r >> x, --r;

        if (l > r) {
            cout << 0 << '\n';
        } else {
            cout << mst.range_frequency(l, r, x) << '\n';
        }
    }
    return 0;
}
