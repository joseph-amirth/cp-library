#define PROBLEM "https://judge.yosupo.jp/problem/persistent_range_affine_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/affine_function.hpp"
#include "algebra/groups/sum.hpp"
#include "algebra/mint/static_mint.hpp"
#include "range_query/segment_tree/persistent_lazy.hpp"

using algebra::affine_function;
using algebra::groups::sum;
using range_query::lazy_persistent_segment_tree;
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

struct persistent_range_affine_range_sum : public lazy_persistent_segment_tree<sum<mint>, range_affine> {
    persistent_range_affine_range_sum(auto &&...args) : lazy_persistent_segment_tree<sum<mint>, range_affine>(args...) {}

    int range_rewire(int version1, int version2, int ql, int qr) {
        assert(0 <= ql && ql <= qr && qr < n);
        auto driver = [&](auto &&self, int i, int j, int l, int r) -> int {
            if (r < ql || qr < l) {
                return i;
            } else if (ql <= l && r <= qr) {
                return j;
            } else {
                push_down(i, l, r);
                push_down(j, l, r);
                int m = (l + r) / 2;
                int left = self(self, nodes[i].left, nodes[j].left, l, m);
                int right = self(self, nodes[i].right, nodes[j].right, m + 1, r);
                return new_node(node(monoid.op(nodes[left].val, nodes[right].val), left, right));
            }
        };
        versions.push_back(driver(driver, version1, version2, 0, n - 1));
        return versions.back();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    std::vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    persistent_range_affine_range_sum pst(a.begin(), a.end());

    std::vector<int> versions = {pst.versions.back()};
    versions.reserve(q + 1);

    while (q--) {
        int t;
        cin >> t;

        if (t == 0) {
            int k, l, r;
            mint b, c;
            cin >> k >> l >> r >> b >> c;
            ++k, --r;
            versions.push_back(pst.range_update(versions[k], l, r, affine_function(b, c)));
        } else if (t == 1) {
            int k, s, l, r;
            cin >> k >> s >> l >> r;
            ++k, ++s, --r;
            versions.push_back(pst.range_rewire(versions[k], versions[s], l, r));
        } else {
            int k, l, r;
            cin >> k >> l >> r;
            ++k, --r;
            cout << pst.range_query(versions[k], l, r) << '\n';
            versions.push_back(-1);
        }
    }
    return 0;
}
