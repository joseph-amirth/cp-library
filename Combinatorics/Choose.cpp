#include <vector>
#include <cassert>
#include "../Numeric/Static-Mint.h"
using namespace std;

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