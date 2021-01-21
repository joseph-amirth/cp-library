#include <vector>
#include <numeric>

struct union_find {
	struct merge_op {
		int l1, l2, r1, r2;
		merge_op() : l1(), l2(), r1(), r2() {}
		merge_op(int l1, int l2, int r1, int r2) : l1(l1), l2(l2), r1(r1), r2(r2) {}
	};

	int n, c;
	std::vector<int> p, r;
	std::vector<merge_op> merges;

	union_find() : n(), c() {}
	union_find(int n) : n(n), c(n), p(n), r(n, 1) {
		std::iota(p.begin(), p.end(), 0);
	}

	int count() const noexcept {
		return c;
	}

	int find(int x) {
		return x == p[x] ? x : find(p[x]);
	}

	bool merge(int x, int y) {
		int l1 = find(x);
		int l2 = find(y);
		if (l1 == l2)
			return false;

		merges.emplace_back(l1, l2, r[l1], r[l2]);
		if (r[l1] >= r[l2]) {
			r[l1] += r[l2];
			p[l2] = l1;
		} else {
			p[l1] = l2;
			r[l2] += r[l1];
		}
		c -= 1;
		return true;
	}

	void save() {
		merges.emplace_back();
	}

	void roll_back() {
		for (; !merges.empty() && merges.back().r1 != 0; c++) {
			merge_op last = merges.back();
			p[last.l1] = last.l1;
			p[last.l2] = last.l2;
			r[last.l1] = last.r1;
			r[last.l2] = last.r2;
			merges.pop_back();
		}
		if (!merges.empty() && merges.back().r1 == 0) {
			merges.pop_back();
		}
	}
};