#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/mint/static_mint.hpp"
#include "polynomials/bitwise_convolution/subset.hpp"

int main() {
    int n;
    cin >> n;

    using mint = algebra::static_mint<998244353>;

    vector<mint> a(1 << n), b(1 << n);
    for (auto &x : a)
        cin >> x;
    for (auto &x : b)
        cin >> x;

    using polynomials::subset_convolution;

    auto c = subset_convolution(a, b);
    for (auto &x : c)
        cout << x << ' ';
    cout << '\n';
}
