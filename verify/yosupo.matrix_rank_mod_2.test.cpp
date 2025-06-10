#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank_mod_2"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "linear_algebra/mod2/matrix.hpp"
#include "linear_algebra/mod2/rank.hpp"

using linear_algebra::mod2::matrix;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    if (m == 0 || n == 0) {
        cout << 0 << '\n';
        return 0;
    }

    matrix a(m, n);
    for (int i = 0; i < m; i++) {
        cin >> a[i];
    }

    cout << linear_algebra::mod2::rank(a) << '\n';
    return 0;
}
