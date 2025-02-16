#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"

#include <bits/stdc++.h>
using namespace std;

#include "strings/suffix_array.hpp"

int main() {
    using namespace strings;

    string s;
    cin >> s;

    for (auto x : suffix_array(s)) {
        cout << x << ' ';
    }
    cout << '\n';
    return 0;
}
