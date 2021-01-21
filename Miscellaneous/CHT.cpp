#include <vector>

template <typename T, typename Q>
struct CHT {
	using line = std::pair<T, T>;

	bool f1(line l1, line l2, line l3) {
		return (Q) (l1.second - l2.second) * (l3.first - l1.first) >=
			   (Q) (l1.second - l3.second) * (l2.first - l1.first);
	}

	bool f2(line l1, line l2, Q x) {
		return (l1.second - l2.second) <= (Q) (l2.first - l1.first) * x;
	}

	std::vector<line> lines;

	void add(T m, T c) {
		line nw(m, c);
		while (lines.size() > 1 and f1(lines[lines.size() - 2], lines[lines.size() - 1], nw)) {
			lines.pop_back();
		}
		lines.push_back(nw);
	}

	Q query(Q x) {
		int lo = 0, hi = lines.size() - 1;
		while (lo < hi) {
			int mid = lo + (hi - lo + 1) / 2;
			if (f2(lines[mid - 1], lines[mid], x))
				lo = mid;
			else hi = mid - 1;
		}
		return (Q) lines[lo].first * x + lines[lo].second;
	}
};