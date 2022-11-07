#define PROBLEM "https://judge.yosupo.jp/problem/two_sat"

#include <bits/stdc++.h>
using namespace std;

#include "newgraphs/two-sat.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s >> s;

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> clauses(m);
    for (auto&[x, y] : clauses) {
        cin >> x >> y >> s;
        x = x < 0 ? -x - 1 + n : x - 1;
        y = y < 0 ? -y - 1 + n : y - 1;
    }

    auto assignment = two_sat(n, clauses);

    if (assignment.empty()) {
        cout << "s UNSATISFIABLE\n";
    } else {
        cout << "s SATISFIABLE\n";
        cout << "v ";
        for (int i = 0; i < n; i++) {
            cout << (assignment[i] ? i + 1 : -i - 1) << ' ';
        }
        cout << 0 << '\n';
    }
    return 0;
}
