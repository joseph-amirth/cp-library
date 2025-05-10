#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "algebra/mint/static_mint.hpp"
#include "linear_algebra/matrix.hpp"

using linear_algebra::matrix;
using mint = algebra::static_mint<998244353>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    matrix<mint> a(n, m), b(m, k);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            cin >> b[i][j];
        }
    }

    auto c = a * b;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            cout << c[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}
