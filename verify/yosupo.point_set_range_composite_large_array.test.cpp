#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite_large_array"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/affine_function.hpp"
#include "algebra/mint/static_mint.hpp"
#include "range_query/segment_tree/implicit.hpp"

using algebra::affine_function;
using algebra::monoids::affine_function_composition;
using range_query::implicit_segment_tree;
using mint = algebra::static_mint<998244353>;

struct id {
    affine_function<mint> operator()(int l, int r) {
        return affine_function<mint>();
    }
};

struct implicit_point_set_range_composite : public implicit_segment_tree<affine_function_composition<mint>, id> {
    implicit_point_set_range_composite(auto &&...args) : implicit_segment_tree<affine_function_composition<mint>, id>(args...) {}

    void point_set(int qi, affine_function<mint> f) {
        update_point(qi, [&f](affine_function<mint> &g) { g = f; });
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    implicit_point_set_range_composite st(n);

    while (q--) {
        int t;
        cin >> t;

        if (t == 0) {
            int p;
            mint c, d;
            cin >> p >> c >> d;
            st.point_set(p, affine_function(c, d));
        } else {
            int l, r;
            mint x;
            cin >> l >> r >> x, --r;
            cout << st.range_query(l, r)(x) << '\n';
        }
    }
    return 0;
}
