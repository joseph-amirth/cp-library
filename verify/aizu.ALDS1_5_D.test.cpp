#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/5/ALDS1_5_D"

#include <bits/stdc++.h>
using namespace std;

#include "misc/inversions.hpp"

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    auto [inv, _] = inversions(std::move(a));
    cout << inv << '\n';
    return 0;
}
