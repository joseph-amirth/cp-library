#define PROBLEM "https://judge.yosupo.jp/problem/factorize"

#include <bits/stdc++.h>
using namespace std;

#include "number_theory/factorize/pollard_rho.hpp"

using ll = long long;
using number_theory::factorize;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    while (q--) {
        ll n;
        cin >> n;

        auto factors = factorize(n);
        sort(factors.begin(), factors.end());

        cout << factors.size() << ' ';
        for (auto factor : factors) {
            cout << factor << ' ';
        }
        cout << endl;
    }
    return 0;
}
