#include <vector>
#include <cassert>
#include "../Numeric/StaticMint.h"
using namespace std;

namespace C1 {
	vector<vector<long long>> C;
	void choose(int n) {
		C.assign(n + 1, vector<long long>(n + 1));
		for (int i = 0; i <= n; i++) {
			for (int j = 0; j <= i; j++) {
				if (j == 0 or j == i)
					C[i][j] = 1;
				else C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
			}
		}
	}
}

template <typename mint>
struct choose {
	vector<mint> fact, inv_fact;
	choose(int n) : fact(n + 1), inv_fact(n + 1) {
		fact[0] = inv_fact[0] = 1;
		for (int i = 1; i <= n; i++) {
			fact[i] = i * fact[i - 1];
			inv_fact[i] = fact[i].inv();
		}
	}

	mint operator()(int n, int k) const {
		assert(0 <= n && n < int(fact.size()) && 0 <= k && k <= n);
		return fact[n] * inv_fact[k] * inv_fact[n - k];
	}
};