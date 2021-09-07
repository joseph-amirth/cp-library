#include "../Numeric/Static-Mint.h"
#include <vector>
#include <cassert>

namespace string_hashing {
	template <int B, int M>
	struct rolling_hash {
		static std::vector<static_mint<M>> pw, inv_pw;
		std::vector<static_mint<M>> pref;

		rolling_hash(const std::string &s) : pref(s.size()) {
			assert(!s.empty());
			while (pw.size() < s.size()) {
				pw.push_back(B * pw.back());
				inv_pw.push_back(pw.back().inv());
			}
			pref[0] = s[0];
			for (int i = 1; i < s.size(); i++) {
				pref[i] = pw[i] * s[i] + pref[i - 1];
			}
		}

		static_mint<M> substr(int i, int j) const {
			return (pref[j] - (i == 0 ? 0 : pref[i - 1])) * inv_pw[i];
		}

		static_mint<M> operator()() const {
			return pref.back();
		}
	};

	template <int B, int M>
	std::vector<static_mint<M>> rolling_hash<B, M>::pw(1, 1);

	template <int B, int M>
	std::vector<static_mint<M>> rolling_hash<B, M>::inv_pw(1, 1);

	template <int B1, int M1, int B2, int M2>
	struct double_rolling_hash {
		rolling_hash<B1, M1> h1;
		rolling_hash<B2, M2> h2;

		double_rolling_hash(const std::string &s) : h1(s), h2(s) { }

		std::pair<static_mint<M1>, static_mint<M2>> substr(int i, int j) const {
			return std::make_pair(h1.substr(i, j), h2.substr(i, j));
		}

		std::pair<static_mint<M1>, static_mint<M2>> operator()() const {
			return std::make_pair(h1.pref.back(), h2.pref.back());
		}
	};
}