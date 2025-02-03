#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_and_convolution"

#include <bits/stdc++.h>
using namespace std;

#include "mint/static_mint.hpp"
#include "polynomial/bitwise_convolution.hpp"

int main() {
    int n;
    cin >> n;

    using mint = static_mint<998244353>;

    vector<mint> a(1 << n), b(1 << n);
    for (auto &x : a)
        cin >> x;
    for (auto &x : b)
        cin >> x;

    using polynomial::and_convolution;

    auto c = and_convolution(a, b);
    for (auto &x : c)
        cout << x << ' ';
    cout << '\n';
}
