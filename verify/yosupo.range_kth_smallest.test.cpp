#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include <bits/stdc++.h>
using namespace std;

#include "data-structures/static-wavelet-tree.hpp"

using data_structures::static_wavelet_tree;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a) cin >> x;

    static_wavelet_tree wt(a.begin(), a.end());

    while (q--) {
        int l, r, k;
        cin >> l >> r >> k, --r;

        cout << wt.range_quantile(l, r, k) << '\n';
    }
    return 0;
}
