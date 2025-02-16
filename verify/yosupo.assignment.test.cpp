#define PROBLEM "https://judge.yosupo.jp/problem/assignment"

#include <bits/stdc++.h>
using namespace std;

#include "networks/hungarian_algorithm.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using networks::hungarian_algorithm;

    int n;
    cin >> n;

    vector<vector<long long>> a(n, vector<long long>(n));
    for (auto &row : a)
        for (auto &cell : row)
            cin >> cell;

    auto [cost, solution] = hungarian_algorithm(a);
    cout << cost << '\n';
    for (int i : solution)
        cout << i << ' ';
    cout << '\n';
}
