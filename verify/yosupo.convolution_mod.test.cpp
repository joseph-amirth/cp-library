#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/mint/static_mint.hpp"
#include "polynomials/ntt.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using mint = algebra::static_mint<998244353>;
    using polynomials::ntt::inplace_convolution;

    int n, m;
    cin >> n >> m;

    vector<mint> a(n), b(m);
    for (auto &x : a)
        cin >> x;
    for (auto &x : b)
        cin >> x;

    inplace_convolution(a, b);
    for (int i = 0; i < n + m - 1; i++) {
        cout << a[i] << ' ';
    }
    cout << '\n';
    return 0;
}
