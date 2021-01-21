#include <vector>

template <typename string_t>
std::pair<std::vector<int>, std::vector<int>> palindromic_substrings(const string_t &s, int n = -1) {
	if (n == -1) n = int(s.length());
	std::vector<int> d_odd(n);
	for (int i = 0, l = 0, r = -1; i < n; i++) {
		int k = (i > r) ? 1 : std::min(d_odd[l + r - i], r - i + 1);
		while (0 <= i - k && i + k < n && s[i - k] == s[i + k]) {
			k++;
		}
		d_odd[i] = k--;
		if (i + k > r) {
			l = i - k;
			r = i + k;
		}
	}
	std::vector<int> d_even(n);
	for (int i = 0, l = 0, r = -1; i < n; i++) {
		int k = (i > r) ? 0 : std::min(d_even[l + r - i + 1], r - i + 1);
		while (0 <= i - k - 1 && i + k < n && s[i - k - 1] == s[i + k]) {
			k++;
		}
		d_even[i] = k--;
		if (i + k > r) {
			l = i - k - 1;
			r = i + k ;
		}
	}
	return std::make_pair(d_odd, d_even);
}