#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/affine_function.hpp"
#include "algebra/groups/sum.hpp"
#include "algebra/mint/static_mint.hpp"
#include "range_query/segment_tree/lazy.hpp"

using mint = algebra::static_mint<998244353>;
using range_query::lazy_segment_tree;

struct range_affine {
    using value_type = algebra::affine_function<mint>;
    using target_type = mint;

    value_type id() {
        return value_type();
    }

    value_type op(value_type &f, value_type &g) {
        return f(g);
    }

    target_type apply(value_type &f, mint x, int size) {
        return f.a * x + f.b * size;
    }

    std::pair<value_type, value_type> split(value_type &f, int size) {
        return std::make_pair(f, f);
    }
};

struct range_affine_range_sum : public lazy_segment_tree<algebra::groups::sum<mint>, range_affine> {
    template <typename... Args>
    range_affine_range_sum(Args &&...args) : lazy_segment_tree<algebra::groups::sum<mint>, range_affine>(std::forward<Args>(args)...) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<mint> a(n);
    for (auto &x : a)
        cin >> x;

    range_affine_range_sum st(a.begin(), a.end());

    while (q--) {
        int type;
        cin >> type;

        if (type == 0) {
            int l, r;
            mint b, c;
            cin >> l >> r >> b >> c, --r;
            st.range_update(l, r, algebra::affine_function<mint>(b, c));
        } else {
            int l, r;
            cin >> l >> r, --r;
            cout << st.range_query(l, r) << '\n';
        }
    }
    return 0;
}
