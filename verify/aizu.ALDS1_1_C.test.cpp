#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_1_C"

#include <bits/stdc++.h>
using namespace std;

#include "number_theory/primality/naive.hpp"

using ll = long long;
using number_theory::is_prime;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int answer = 0;
    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;
        answer += is_prime(p);
    }
    cout << answer << '\n';
    return 0;
}
