#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_palindromes"

#include <bits/stdc++.h>
using namespace std;

#include "strings/manacher.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace strings;

    string s;
    cin >> s;

    auto [odd, even] = count_palindromes(s);
    for (int i = 0; i + 1 < (int)s.length(); i++) {
        cout << 2 * odd[i] - 1 << ' ' << 2 * even[i + 1] << ' ';
    }
    cout << 2 * odd.back() - 1 << '\n';
    return 0;
}
