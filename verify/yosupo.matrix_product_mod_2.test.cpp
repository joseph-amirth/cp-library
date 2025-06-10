#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product_mod_2"

#include <bits/stdc++.h>
using namespace std;

#include "linear_algebra/mod2/matrix.hpp"

using ll = long long;
using linear_algebra::mod2::matrix;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    matrix a(n, m), b(m, k);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < m; i++) {
        cin >> b[i];
    }

    auto c = a * b;
    for (int i = 0; i < n; i++) {
        cout << c[i] << '\n';
    }
    return 0;
}
