#include <vector>

template<typename string_t>
std::vector<int> lps(const string_t &t, int n = -1) {
	if (n == -1) n = int(t.size());
	std::vector<int> lps(n);
	int len = 0, i = 1;
	lps[0] = 0;
	while (i < n) {
		if (t[i] == t[len]) {
			len += 1;
			lps[i] = len;
			i += 1;
		} else if (len != 0) {
			len = lps[len - 1];
		} else {
			lps[i] = 0;
			i += 1;
		}
	}
	return lps;
}

template<typename string_t>
std::vector<int> kmp(const string_t &s, int n, const string_t &t, int m) {
	std::vector<int> l = lps(t, m), match;
	int i = 0, j = 0;

	while (i < n) {
		if (s[i] == t[j]) {
			i += 1;
			j += 1;
		}
		if (j == m) {
			match.push_back(i - j);
			j = l[j - 1];
		} else if (i < n and s[i] != t[j]) {
			if (j != 0)
				j = l[j - 1];
			else i += 1;
		}
	}
	return match;
}

template<typename string_t>
std::vector<std::vector<int>> prefix_automaton(const string_t &s, int n = -1) {
	if (n == -1) n = int(s.size());
	std::vector<int> l = lps(s, n);
	std::vector<std::vector<int>> f(n, std::vector<int>(26));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 26; j++) {
			if (j == s[i] - 'a' or i == 0) {
				f[i][j] = i;
			} else {
				f[i][j] = f[l[j - 1]][j];
			}
		}
	}
	return f;
}