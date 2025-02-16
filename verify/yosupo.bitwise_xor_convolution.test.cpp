#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/mint/static_mint.hpp"
#include "polynomials/bitwise_convolution/xor.hpp"

int main() {
    int n;
    cin >> n;

    using mint = algebra::static_mint<998244353>;

    vector<mint> a(1 << n), b(1 << n);
    for (auto &x : a)
        cin >> x;
    for (auto &x : b)
        cin >> x;

    using polynomials::xor_convolution;

    auto c = xor_convolution(a, b);
    for (auto &x : c)
        cout << x << ' ';
    cout << '\n';
}
