#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "algebra/mint/static_mint.hpp"
#include "linear_algebra/determinant.hpp"
#include "linear_algebra/matrix.hpp"

using linear_algebra::determinant;
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

    cout << determinant(a) << '\n';
    return 0;
}
