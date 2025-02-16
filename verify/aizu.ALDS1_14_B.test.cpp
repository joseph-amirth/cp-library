#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B"

#include <bits/stdc++.h>
using namespace std;

#include "strings/kmp.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace strings;

    string s, t;
    cin >> s >> t;

    for (auto occurrence : kmp_pattern_matching(s, t)) {
        cout << occurrence << '\n';
    }
    return 0;
}
