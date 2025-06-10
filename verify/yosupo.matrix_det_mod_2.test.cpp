#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det_mod_2"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#include "linear_algebra/mod2/determinant.hpp"
#include "linear_algebra/mod2/matrix.hpp"

using linear_algebra::mod2::determinant;
using linear_algebra::mod2::matrix;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    matrix a(n, n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    cout << determinant(a) << '\n';
    return 0;
}
