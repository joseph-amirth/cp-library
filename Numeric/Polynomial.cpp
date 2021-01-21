#include "StaticMint.h"
#include <vector>
#include <algorithm>

namespace polynomial_algebra {
	namespace ntt {
		template <int P>
		struct prime_info {
			constexpr static int root = 0, root_pw = 0;
		};

		template <> struct prime_info<7340033>   { constexpr static int root = 5, root_pw = 1 << 20; };
		template <> struct prime_info<998244353> { constexpr static int root = 15311432, root_pw = 1 << 23; };
		template <> struct prime_info<754974721> { constexpr static int root = 739831874, root_pw = 1 << 24; };
		template <> struct prime_info<167772161> { constexpr static int root = 243, root_pw = 1 << 25; };
		template <> struct prime_info<469762049> { constexpr static int root = 2187, root_pw = 1 << 26; };

		std::vector<int> rev = {0};

		void compute_bit_reverse(int lg) {
			static int computed = 0;
			if (lg <= computed) return;
			rev.resize(1 << lg);
			for (int i = (1 << computed); i < (1 << lg); i++) {
				rev[i] = (rev[i >> 1] >> 1) ^ ((i & 1) << 30);
			}
			computed = lg;
		}

		template <int M>
		std::vector<static_mint<M>> root = {0, 1};

		template <int M>
		void compute_roots(int lg) {
			static int computed = 1;
			if (lg <= computed) return;
			root<M>.resize(1 << lg);
			for (int k = computed; k < lg; k++) {
				static_mint<M> z(prime_info<M>::root);
				for (int i = (1 << (k + 1)); i < prime_info<M>::root_pw; i <<= 1) {
					z *= z;
				}
				for (int i = (1 << (k - 1)); i < (1 << k); i++) {
					root<M>[i << 1] = root<M>[i];
					root<M>[i << 1 | 1] = root<M>[i] * z;
				}
			}
		}

		template<int M>
		void ntt(std::vector<static_mint<M>> &a) {
			int n = int(a.size()), lg = 32 - __builtin_clz(n) - 1;
			compute_bit_reverse(lg);
			compute_roots<M>(lg);
			int shift = 31 - lg;
			for (int i = 0; i < n; i++) {
				if (i < (rev[i] >> shift)) {
					std::swap(a[i], a[rev[i] >> shift]);
				}
			}
			for (int k = 1; k < n; k <<= 1) {
				static_mint<M> wl = prime_info<M>::root;
				for (int i = 2 * k; i < prime_info<M>::root_pw; i <<= 1) {
					wl *= wl;
				}
				for (int i = 0; i < n; i += 2 * k) {
					for (int j = 0; j < k; j++) {
						static_mint<M> z = root<M>[j + k] * a[i + j + k];
						a[i + j + k] = a[i + j] - z;
						a[i + j] = a[i + j] + z;
					}
				}
			}
		}
	}

	template <typename T>
	struct polynomial : public std::vector<T> {
		template <typename...args>
		polynomial(args...A) : std::vector<T>(A...) { }

		polynomial(const std::initializer_list<T> &l) : std::vector<T>(l) { }

		int deg() const noexcept {
			return (int) this->size() - 1;
		}

		void normalize() {
			for (int i = deg(); i >= 0; i--) {
				if ((*this)[i]) {
					this->resize(i + 1);
					return;
				}
			}
			this->clear();
		}

		polynomial &operator+=(const polynomial &q) {
			if (q.size() > this->size()) {
				this->resize(q.size());
			}
			for (int i = 0; i < q.size(); i++) {
				(*this)[i] += q[i];
			}
			normalize();
			return *this;
		}

		polynomial &operator-=(const polynomial &q) {
			if (q.size() > this->size()) {
				this->resize(q.size());
			}
			for (int i = 0; i < q.size(); i++) {
				(*this)[i] -= q[i];
			}
			normalize();
			return *this;
		}

		void naive_mul(polynomial &a, const polynomial &b) const {
			polynomial result(a.deg() + b.deg() + 1);
			for (int i = 0; i <= a.deg(); i++) {
				for (int j = 0; j <= b.deg(); j++) {
					result[i + j] += a[i] * b[j];
				}
			}
			a.swap(result);
		}

		void ntt_mul(polynomial &fa, polynomial fb) const {
			int n = 1;
			while (n < fa.size() + fb.size()) {
				n <<= 1;
			}
			fa.resize(n);
			fb.resize(n);
			ntt::ntt(fa), ntt::ntt(fb);
			T n_inv = T(n).inv();
			for (int i = 0; i < n; i++) {
				fa[i] *= fb[i] * n_inv;
			}
			std::reverse(fa.begin() + 1, fa.end());
			ntt::ntt(fa);
		}

		polynomial &operator*=(const polynomial &q) {
			if (this->empty() || q.empty())
				this->clear();
			else if (this->size() <= 60)
				naive_mul(*this, q);
			else ntt_mul(*this, q);
			normalize();
			return *this;
		}

		polynomial &operator*=(const T &val) {
			for (auto &x : *this) {
				x *= val;
			}
			return *this;
		}

		void divide(polynomial &a, polynomial b) const {
			assert(!b.empty());
			if (a.deg() < b.deg()) {
				a.clear();
				return;
			}
			reverse(a.begin(), a.end());
			int sz = a.deg() - b.deg() + 1;
			a %= sz;
			reverse(b.begin(), b.end());
			polynomial d = a * b.inv(sz);
			d %= sz;
			reverse(d.begin(), d.end());
			a.swap(d);
		}

		polynomial &operator/=(const polynomial &q) {
			divide(*this, q);
			normalize();
			return *this;
		}

		polynomial &operator/=(const T &val) {
			for (auto &x : *this) {
				x /= val;
			}
			return *this;
		}

		polynomial &operator%=(const polynomial &q) {
			*this = *this - q * (*this / q);
			normalize();
			return *this;
		}

		polynomial &operator%=(size_t k) {
			if (k <= deg())
				this->resize(k);
			return *this;
		}

		polynomial &operator<<=(size_t k) {
			if (this->size() <= k) {
				this->clear();
			} else {
				polynomial result(this->begin() + k, this->end());
				this->swap(result);
			}
			return *this;
		}

		polynomial &operator>>=(size_t k) {
			polynomial result(this->size() + k);
			std::copy(this->begin(), this->end(), result.begin() + k);
			this->swap(result);
			return *this;
		}

		friend polynomial operator+(polynomial p, const polynomial &q) {
			p += q;
			return p;
		}

		friend polynomial operator-(polynomial p, const polynomial &q) {
			p -= q;
			return p;
		}

		friend polynomial operator*(polynomial p, const polynomial &q) {
			p *= q;
			return p;
		}

		friend polynomial operator*(polynomial p, const T &val) {
			for (auto &x : p) {
				x *= val;
			}
			return p;
		}

		friend polynomial operator*(const T &val, polynomial p) {
			for (auto &x : p) {
				x *= val;
			}
			return p;
		}

		friend polynomial operator/(polynomial p, const polynomial &q) {
			p /= q;
			return p;
		}

		friend polynomial operator/(polynomial p, const T &val) {
			for (auto &x : p) {
				x /= val;
			}
			return p;
		}

		friend polynomial operator%(polynomial p, const polynomial &q) {
			p %= q;
			return p;
		}

		friend polynomial operator%(polynomial p, size_t k) {
			p %= k;
			return p;
		}

		friend polynomial operator<<(polynomial p, size_t k) {
			p <<= k;
			return p;
		}

		friend polynomial operator>>(polynomial p, size_t k) {
			p >>= k;
			return p;
		}

		polynomial inv(int k = -1) const {
			if (k == -1) k = this->size();
			assert(!this->empty() && (*this)[0] != 0);
			polynomial b(1, 1 / (*this)[0]);
			for (int i = 2; i <= (k << 1); i <<= 1) {
				polynomial temp = (*this) % i;
				temp *= b;
				temp %= i;
				temp = polynomial(1, 2) - temp;
				b *= temp;
				b %= i;
			}
			b.resize(k);
			return b;
		}

		polynomial derivative() const {
			if (deg() < 1) {
				return {};
			}
			polynomial result(this->size() - 1);
			for (int i = 1; i < this->size(); i++) {
				result[i - 1] = i * (*this)[i];
			}
			return result;
		}

		polynomial integral() const {
			if (this->empty()) {
				return {};
			}
			polynomial result(this->size() + 1);
			for (int i = 0; i < this->size(); i++) {
				result[i + 1] = (*this)[i] / (i + 1);
			}
			return result;
		}

		polynomial log(int k = -1) const {
			assert(!this->empty() && (*this)[0] == 1);
			if (k == -1) k = this->size();
			polynomial result = ((derivative() % k) * inv(k)).integral();
			result.resize(k);
			return result;
		}

		polynomial exp(int k = -1) const {
			assert(this->empty() || (*this)[0] == 0);
			if (k == -1) k = this->size();
			polynomial q(1, 1);
			for (int i = 2; i <= (k << 1); i <<= 1) {
				polynomial temp = polynomial(1, 1) + (*this % i) - q.log(i);
				q *= temp;
				q %= i;
			}
			q.resize(k);
			return q;
		}

		polynomial pow(int n, int k = -1) const {
			if (k == -1)
				k = this->size();
			if (this->empty())
				return polynomial(k);
			T alpha = 0;
			int pw = 0;
			for (int i = 0; i < this->size(); i++) {
				if ((*this)[i]) {
					alpha = (*this)[i];
					pw = i;
					break;
				}
			}
			if ((long long)pw * n >= k) {
				return polynomial(k);
			}
			polynomial<T> b = (*this) << pw;
			b /= alpha;
			b = (n * b.log(k)).exp(k);
			b >>= pw * n;
			b *= alpha.pow(n);
			b.resize(k);
			return b;
		}
	};
}