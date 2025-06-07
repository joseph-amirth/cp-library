#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include <bits/stdc++.h>
using namespace std;

#include "number_theory/primality/miller_rabin.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using number_theory::is_prime;

    int q;
    cin >> q;

    while (q--) {
        long long n;
        cin >> n;

        cout << (is_prime(n) ? "Yes" : "No") << '\n';
    }
}
