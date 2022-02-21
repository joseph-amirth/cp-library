#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"

#include <bits/stdc++.h>
using namespace std;

#include "mint/static-mint.hpp"
#include "data-structures/splay-tree/implicit-splay-tree.hpp"

using mint = static_mint<998244353>;

struct splay_node : implicit_splay_tree<splay_node> {
	mint value;
	mint aggregate;

	mint prod, add;
	bool flipped;

	splay_node(int x = 0) {
		value = x, aggregate = x;
		prod = 1, add = 0;
		flipped = false;

		size = 1;
		parent = left = right = nullptr;
	}

	virtual void push_down() {
		if (prod != 1 || add != 0) {
			if (left != nullptr) {
				left->value = prod * left->value + add;
				left->aggregate = prod * left->aggregate + left->size * add;
				left->prod *= prod;
				left->add = prod * left->add + add;
			}
			if (right != nullptr) {
				right->value = prod * right->value + add;
				right->aggregate = prod * right->aggregate + right->size * add;
				right->prod *= prod;
				right->add = prod * right->add + add;
			}
			prod = 1, add = 0;
		}

		if (flipped) {
			std::swap(left, right);
			if (left != nullptr) {
				left->flipped ^= true;
			}
			if (right != nullptr) {
				right->flipped ^= true;
			}
			flipped = false;
		}
	}

	virtual void pull_up() {
		implicit_splay_tree<splay_node>::pull_up();
		aggregate = value;
		if (left != nullptr) {
			aggregate = left->aggregate + aggregate;
		}
		if (right != nullptr) {
			aggregate = aggregate + right->aggregate;
		}
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	cin >> n >> q;

	vector<int> a(n);
	for (auto &x : a) cin >> x;

	splay_node *root = splay_node::build_tree(a.begin(), a.end());

	while (q--) {
		int type;
		cin >> type;

		if (type == 0) {
			int i, x;
			cin >> i >> x;
			root = splay_node::insert(root, i, x);
		} else if (type == 1) {
			int i;
			cin >> i;
			root = splay_node::erase(root, i);
		} else if (type == 2) {
			int l, r;
			cin >> l >> r, --r;
			root = splay_node::query(root, l, r, [&](splay_node *x) {
				x->flipped ^= true;
			});
		} else if (type == 3) {
			int l, r; mint b, c;
			cin >> l >> r >> b >> c, --r;
			root = splay_node::query(root, l, r, [&](splay_node *x) {
				x->value = b * x->value + c;
				x->aggregate = b * x->aggregate + x->size * c;
				x->prod *= b;
				x->add = b * x->add + c;
			});
		} else if (type == 4) {
			int l, r;
			cin >> l >> r, --r;
			mint ans;
			root = splay_node::query(root, l, r, [&](splay_node *x) {
				ans = x->aggregate;
			});
			cout << ans << '\n';
		}
	}
	return 0;
}
