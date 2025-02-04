#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/affine_function.hpp"
#include "algebra/groups/sum.hpp"
#include "algebra/mint/static_mint.hpp"
#include "range_query/segment_tree/lazy.hpp"

using mint = algebra::static_mint<998244353>;
using range_query::lazy_segment_tree;

struct lazy_update {
    using tag_type = algebra::affine_function<mint>;

    static tag_type e() {
        return tag_type();
    }

    static void apply(tag_type &lazy_val, mint &value, tag_type &tag, int l, int r) {
        value = lazy_val.a * value + (r - l + 1) * lazy_val.b;
        tag = lazy_val(tag);
    }
};

struct range_affine_range_sum : public lazy_segment_tree<algebra::groups::sum<mint>, lazy_update> {
    template <typename... Args>
    range_affine_range_sum(Args &&...args) : lazy_segment_tree<algebra::groups::sum<mint>, lazy_update>(std::forward<Args>(args)...) {}

    void range_affine(int l, int r, algebra::affine_function<mint> f) {
        split_range<true>(l, r, [&f, this](int i, int l, int r) {
            lazy_update::apply(f, t[i], tag[i], l, r);
        });
    }
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
            st.range_affine(l, r, algebra::affine_function<mint>(b, c));
        } else {
            int l, r;
            cin >> l >> r, --r;
            cout << st.range_query(l, r) << '\n';
        }
    }
    return 0;
}
