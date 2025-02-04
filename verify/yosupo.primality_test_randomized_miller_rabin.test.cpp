#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include <bits/stdc++.h>
using namespace std;

#include "number_theory/primality_testing.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using number_theory::randomized_miller_rabin;

    int q;
    cin >> q;

    while (q--) {
        long long n;
        cin >> n;

        cout << (randomized_miller_rabin<30>(n) ? "Yes" : "No") << '\n';
    }
}
