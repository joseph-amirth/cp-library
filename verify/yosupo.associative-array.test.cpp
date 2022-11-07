#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"

#include <bits/stdc++.h>
using namespace std;

#include "hashing/perfect-hashing.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto random_odd_number = [](void) -> uint64_t {
        static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uint64_t x = 0;
        while (x % 2 == 0) {
            x = rng();
        }
        return x;
    };

    auto random_hash_function = [&](int m) {
        return [m, a = random_odd_number()](uint64_t x) -> int {
            return (a * x) >> (64 - m);
        };
    };

    using hash_table = hashing::linear_perfect_hash_table<uint64_t, uint64_t>;

    int q;
    cin >> q;

    vector<uint64_t> keys(q), values(q, -1);
    for (int i = 0; i < q; i++) {
        int type;
        cin >> type >> keys[i];
        if (type == 0) {
            cin >> values[i];
        }
    }

    auto sorted = keys;
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());

    hash_table mp(sorted, random_hash_function);

    for (int i = 0; i < q; i++) {
        if (values[i] == -1) {
            cout << mp[keys[i]] << '\n';
        } else {
            mp[keys[i]] = values[i];
        }
    }
    return 0;
}
