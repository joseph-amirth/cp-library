#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"

#include <bits/stdc++.h>
using namespace std;

#include "strings/pattern-matching/z-function.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace strings;

    string s;
    cin >> s;

    for (int l : z_function(s)) {
        cout << l << ' ';
    }
    cout << '\n';
    return 0;
}
