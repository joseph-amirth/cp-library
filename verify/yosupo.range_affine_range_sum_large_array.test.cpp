#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum_large_array"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/affine_function.hpp"
#include "algebra/groups/sum.hpp"
#include "algebra/mint/static_mint.hpp"
#include "range_query/segment_tree/implicit_lazy.hpp"

using algebra::affine_function;
using algebra::groups::sum;
using range_query::lazy_implicit_segment_tree;
using mint = algebra::static_mint<998244353>;

struct range_affine {
    using value_type = affine_function<mint>;
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

struct zero {
    mint operator()(int l, int r) {
        return 0;
    }
};

struct implicit_range_affine_range_sum : public lazy_implicit_segment_tree<sum<mint>, zero, range_affine> {
    implicit_range_affine_range_sum(auto &&...args) : lazy_implicit_segment_tree<sum<mint>, zero, range_affine>(args...) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    implicit_range_affine_range_sum st(n);

    while (q--) {
        int t;
        cin >> t;

        if (t == 0) {
            int l, r;
            mint b, c;
            cin >> l >> r >> b >> c, --r;
            st.range_update(l, r, affine_function(b, c));
        } else {
            int l, r;
            cin >> l >> r, --r;
            cout << st.range_query(l, r) << '\n';
        }
    }
    return 0;
}
