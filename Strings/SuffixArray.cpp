#include <vector>
#include <algorithm>
#include <string>
#include <numeric>

namespace huh {
	void sa_is(const std::vector<int> &s, std::vector<int> &sa, int sigma) {
		int n = int(s.size());
		std::vector<int> b(sigma);

		bool base = true;
		for (int i = 0; i < n; i++) {
			b[s[i]]++;
			base &= b[s[i]] == 1;
		}
		if (base) {
			for (int i = 1; i < sigma; i++)
				b[i] += b[i - 1];
			for (int i = 0; i < n; i++)
				sa[--b[s[i]]] = i;
			return;
		}

		std::vector<bool> t(n);
		for (int i = n - 2; i >= 0; i--) {
			if (s[i] == s[i + 1]) {
				t[i] = t[i + 1];
			} else if (s[i] < s[i + 1]) {
				t[i] = true;
			}
		}

		std::vector<int> bkt(sigma), g(n, -1);
		std::partial_sum(b.begin(), b.end(), bkt.begin());
		for (int i = 1; i < n; i++) {
			if (t[i] && !t[i - 1]) {
				g[--bkt[s[i]]] = i;
			}
		}

		auto induce = [&](std::vector<int> &sa) -> void {
			bkt[0] = 0;
			std::partial_sum(b.begin(), b.end() - 1, bkt.begin() + 1);
			if (!t[n - 1]) {
				sa[bkt[s[n - 1]]++] = n - 1;
			}
			for (int i = 0; i < n; i++) {
				if (sa[i] > 0 && !t[sa[i] - 1]) {
					sa[bkt[s[sa[i] - 1]]++] = sa[i] - 1;
				}
			}
			std::partial_sum(b.begin(), b.end(), bkt.begin());
			for (int i = n - 1; i >= 0; i--) {
				if (sa[i] > 0 && t[sa[i] - 1]) {
					sa[--bkt[s[sa[i] - 1]]] = sa[i] - 1;
				}
			}
			if (t[n - 1]) {
				sa[--bkt[s[n - 1]]] = n - 1;
			}
		};

		induce(g);

		auto is_equal = [&](int i, int j) -> bool {
			if (s[i] != s[j])
				return false;
			for (i++, j++; i < s.size() && j < s.size(); i++, j++) {
				if (s[i] != s[j] || t[i] != t[j])
					return false;
				if (i && t[i] && !t[i - 1] && j && t[j] && !t[j - 1])
					return true;
				if ((i && t[i] && !t[i - 1]) != (j && t[j] && !t[j - 1]))
					return false;
			}
			return false;
		};

		std::vector<int> s1(n, -1), p1;
		int last = -1, sigma1 = 0;
		for (int i = 0; i < n; i++) {
			if (g[i] > 0 && t[g[i]] && !t[g[i] - 1]) {
				if (last == -1) {
					s1[g[i]] = sigma1;
				} else {
					if (!is_equal(last, g[i]))
						sigma1++;
					s1[g[i]] = sigma1;
				}
				last = g[i];
			}
		}
		for (int i = 0; i < n; i++) {
			if (s1[i] != -1) {
				p1.push_back(i);
			}
		}
		s1.erase(std::remove_if(s1.begin(), s1.end(), [&](int i) { return i == -1; }), s1.end());

		std::vector<int> sa1(s1.size(), -1);
		sa_is(s1, sa1, sigma1 + 1);

		std::partial_sum(b.begin(), b.end(), bkt.begin());
		for (int i = sa1.size() - 1; i >= 0; i--) {
			sa[--bkt[s[p1[sa1[i]]]]] = p1[sa1[i]];
		}
		induce(sa);
	}

	std::vector<int> suffix_array(const std::string &s) {
		std::vector<int> sa(s.length(), -1);
		sa_is(std::vector<int>(s.begin(), s.end()), sa, 256);
		return sa;
	}

	std::vector<int> suffix_array(const std::vector<int> &s, int sigma) {
		std::vector<int> sa(s.size(), -1);
		sa_is(s, sa, sigma);
		return sa;
	}

	std::vector<int> longest_common_prefix(const std::string &s, const std::vector<int> &sa) {
		int n = s.length();
		std::vector<int> lcp(n - 1), rank(n);
		for (int i = 0; i < n; i++) {
			rank[sa[i]] = i;
		}
		int k = 0;
		for (int i = 0; i < n; i++) {
			if (rank[i] == n - 1) {
				k = 0;
				continue;
			}
			int j = sa[rank[i] + 1];
			while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
				k++;
			}
			lcp[rank[i]] = k;
			if (k) {
				k--;
			}
		}
		return lcp;
	}
}