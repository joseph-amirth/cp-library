#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <bits/stdc++.h>
using namespace std;

#include "data-structures/tensor/flat-tensor.hpp"
#include "data-structures/sparse-table/nd-flat-sparse-table.hpp"

int main() {
    int n, q;
    cin >> n >> q;

    flat_tensor<int, 1> a({n});
    for (int i = 0; i < n; i++) {
        cin >> a.data[i];
    }

    nd_flat_sparse_table<int> st(a, [](int x, int y) { return std::min(x, y); });

    while (q--) {
        int l, r;
        cin >> l >> r;

        cout << st.query({{l, r - 1}}) << '\n';
    }
    return 0;
}
