#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_matrix"

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

    int n;
    ll k;
    cin >> n >> k;

    matrix<mint> a(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }

    auto c = a.pow(k);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << c[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}
