#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "algebra/mint/static_mint.hpp"
#include "linear_algebra/matrix.hpp"
#include "linear_algebra/rank.hpp"

using linear_algebra::matrix;
using mint = algebra::static_mint<998244353>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    if (m == 0 || n == 0) {
        cout << 0 << '\n';
        return 0;
    }

    matrix<mint> a(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }

    cout << linear_algebra::rank(a) << '\n';
    return 0;
}
