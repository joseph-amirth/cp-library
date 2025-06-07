#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"

#include <bits/stdc++.h>
using namespace std;

#include "number_theory/factorize/pollard_rho.hpp"
#include "number_theory/primitive_root.hpp"

using ll = long long;
using number_theory::factorize;
using number_theory::primitive_root;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    while (q--) {
        ll n;
        cin >> n;

        cout << primitive_root(n, [](ll x) {
            return factorize(x);
        }) << '\n';
    }
    return 0;
}
