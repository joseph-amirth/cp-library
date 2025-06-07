#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/1/NTL_1_A"

#include <bits/stdc++.h>
using namespace std;

#include "number_theory/factorize/naive.hpp"

using ll = long long;
using number_theory::factorize;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    cout << n << ':';
    for (auto factor : factorize(n)) {
        cout << ' ' << factor;
    }
    cout << '\n';
    return 0;
}
