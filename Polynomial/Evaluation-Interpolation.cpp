#include "Polynomial.h"

namespace evaluation_and_interpolation {
	template <typename T, typename U>
	std::vector<polynomial<T>> product_tree(const std::vector<U> &x) {
		int n = (int) x.size();
		std::vector<polynomial<T>> tree(2 * n);
		for (int i = 0; i < n; i++) {
			tree[i + n] = {-T(x[i]), T(1)};
		}
		for (int i = n - 1; i > 0; i--) {
			tree[i] = tree[i << 1] * tree[i << 1 | 1];
		}
		return tree;
	}

	template <typename T, typename U>
	std::enable_if_t<!std::is_same<polynomial<T>, U>::value, std::vector<T>>
	multipoint_evaluation(const polynomial<T> &p, const std::vector<U> &x) {
		int n = (int) x.size();

		auto tree = product_tree<T>(x);

		tree[1] = p % tree[1];
		for (int i = 1; i < n; i++) {
			tree[i << 1] = tree[i] % tree[i << 1];
			tree[i << 1 | 1] = tree[i] % tree[i << 1 | 1];
		}

		std::vector<T> result(n);
		for (int i = 0; i < n; i++) {
			result[i] = (tree[i + n].empty() ? T(0) : tree[i + n][0]);
		}
		return result;
	}

	template <typename T, typename U>
	std::enable_if_t<std::is_same<polynomial<T>, U>::value, std::vector<T>>
	multipoint_evaluation(const polynomial<T> &p, const std::vector<U> &tree) {
		int n = (int) tree.size() / 2;
		std::vector<T> result(n);
		auto recurse = [&](int i, polynomial<T> q, const auto &self) -> void {
			q %= tree[i];
			if (i >= n) {
				result[i - n] = q.empty() ? T(0) : q[0];
			} else {
				self(i << 1, q, self);
				self(i << 1 | 1, q, self);
			}
		};
		recurse(1, p, recurse);
		return result;
	}

	template <typename T, typename U>
	std::enable_if_t<!std::is_same<polynomial<T>, U>::value, polynomial<T>>
	interpolation(const std::vector<U> &x, const std::vector<U> &y) {
		int n = (int) x.size();

		auto tree = product_tree<T>(x);
		auto ps = multipoint_evaluation(tree[1].derivative(), tree);

		auto recurse = [&](int i, const auto &self) -> polynomial<T> {
			if (i >= n) {
				return {T(y[i - n]) / ps[i - n]};
			} else {
				return self(i << 1, self) * tree[i << 1 | 1] + self(i << 1 | 1, self) * tree[i << 1];
			}
		};

		return recurse(1, recurse);
	}

	template <typename T, typename U>
	std::enable_if_t<std::is_same<polynomial<T>, U>::value, polynomial<T>>
	interpolation(const std::vector<U> &tree, const std::vector<U> &y) {
		int n = (int) tree.size() / 2;
		auto ps = multipoint_evaluation(tree[1].derivative(), tree);
		auto recurse = [&](int i, const auto &self) -> polynomial<T> {
			if (i >= n) {
				return {T(y[i - n]) / ps[i - n]};
			} else {
				return self(i << 1, self) * tree[i << 1 | 1] + self(i << 1 | 1, self) * tree[i << 1];
			}
		};
		return recurse(1, recurse);
	}
}
