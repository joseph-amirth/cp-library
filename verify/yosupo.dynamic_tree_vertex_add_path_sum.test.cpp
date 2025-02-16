#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum"

#include <bits/stdc++.h>
using namespace std;

#include "trees/link_cut_tree.hpp"

struct lct_node : trees::basic_lct_node<lct_node> {
    long long value, aggregate;

    lct_node(int u) : basic_lct_node<lct_node>(u) {}

    virtual void pull_up() {
        basic_lct_node::pull_up();
        aggregate = value +
                    (left == nullptr ? 0 : left->aggregate) +
                    (right == nullptr ? 0 : right->aggregate);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    trees::link_cut_tree<lct_node> lct(n);
    for (int i = 0; i < n; i++) {
        cin >> lct[i]->value;
        lct[i]->aggregate = lct[i]->value;
    }

    for (int i = 0, u, v; i < n - 1; i++) {
        cin >> u >> v;
        lct.link(u, v);
    }

    while (q--) {
        int type;
        cin >> type;

        if (type == 0) {
            int u, v, w, x;
            cin >> u >> v >> w >> x;
            lct.cut(u, v), lct.link(w, x);
        } else if (type == 1) {
            int p, x;
            cin >> p >> x;
            lct.access(p);
            lct[p]->value += x;
            lct[p]->aggregate += x;
        } else if (type == 2) {
            int u, v;
            cin >> u >> v;
            lct.make_root(u);
            lct.access(v);
            cout << lct[v]->aggregate << '\n';
        } else {
            assert(false);
        }
    }
    return 0;
}
