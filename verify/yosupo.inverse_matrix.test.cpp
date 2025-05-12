#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "algebra/mint/static_mint.hpp"
#include "linear_algebra/inv.hpp"
#include "linear_algebra/matrix.hpp"

using linear_algebra::matrix;
using mint = algebra::static_mint<998244353>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    matrix<mint> a(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }

    auto maybe_inv = linear_algebra::inv(a);
    if (!maybe_inv.has_value()) {
        cout << -1 << '\n';
    } else {
        auto &inv = maybe_inv.value();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << inv[i][j] << ' ';
            }
            cout << '\n';
        }
    }
    return 0;
}
