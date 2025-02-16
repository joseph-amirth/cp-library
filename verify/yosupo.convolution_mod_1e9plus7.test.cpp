#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/mint/montgomery_mint.hpp"
#include "polynomials/ntt.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using mint = algebra::montgomery_mint<1000000007>;
    using polynomials::ntt::arbitrary_mod_convolution;

    int n, m;
    cin >> n >> m;

    vector<mint> a(n), b(m);
    for (auto &x : a)
        cin >> x;
    for (auto &x : b)
        cin >> x;

    auto c = arbitrary_mod_convolution<mint, algebra::montgomery_mint>(a, b);
    for (int i = 0; i < n + m - 1; i++) {
        cout << c[i] << ' ';
    }
    cout << '\n';
    return 0;
}
