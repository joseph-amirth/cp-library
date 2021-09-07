#include <iostream>

template<uint32_t N>
struct montgomery_mint {
	static_assert(0 < N && N % 2 != 0, "Module must be positive and odd");

	constexpr static auto get_n() {
		return N;
	}

	constexpr static auto get_r() {
		return -N % N;
	}

	constexpr static auto get_r_inverse() {
		constexpr auto r = get_r();
		uint32_t r_inverse = 1, r_pow = r;
		for (uint32_t n = N - 2; n != 0; n /= 2) {
			if (n % 2 != 0) {
				r_inverse = (uint64_t) r_inverse * r_pow % N;
			}
			r_pow = (uint64_t) r_pow * r_pow % N;
		}
		return r_inverse;
	}

	constexpr static auto get_r_squared() {
		return (uint64_t)get_r() * get_r() % N;
	}

	constexpr static auto get_n_dash() {
		constexpr auto r_inverse = get_r_inverse();
		return (uint32_t) ((((uint64_t)r_inverse << 32) - 1) / N);
	}

	constexpr static uint32_t reduce(uint64_t x) {
		constexpr auto n_dash = get_n_dash();
		uint32_t y = (x + (uint64_t)((uint32_t) x * n_dash) * N) >> 32;
		return y >= N ? y - N : y;
	}

	uint32_t val;

	constexpr montgomery_mint() : val() {}

	constexpr montgomery_mint(long long x) {
		constexpr auto r_squared = get_r_squared();
		x %= (int) N;
		val = reduce((x < 0 ? x + N : x) * r_squared);
	}

	constexpr uint32_t get_val() const {
		return reduce(val);
	}

	constexpr montgomery_mint pow(uint64_t n) const {
		montgomery_mint ans = 1, x(*this);
		for (; n != 0; n /= 2) {
			if (n & 1) ans *= x;
			x *= x;
		}
		return ans;
	}

	constexpr montgomery_mint inv() const {
		return pow(N - 2);
	}

	constexpr montgomery_mint operator+() const {
		montgomery_mint m;
		m.val = val;
		return m;
	}

	constexpr montgomery_mint operator-() const {
		montgomery_mint m;
		m.val = (val == 0 ? 0 : N - val);
		return m;
	}

	constexpr montgomery_mint &operator+=(const montgomery_mint &m) {
		if ((val += m.val) >= N) val -= N;
		return *this;
	}

	constexpr montgomery_mint &operator-=(const montgomery_mint &m) {
		val = (val < m.val ? val + N - m.val : val - m.val);
		return *this;
	}

	constexpr montgomery_mint &operator*=(const montgomery_mint &m) {
		val = reduce((uint64_t)val * m.val);
		return *this;
	}

	constexpr montgomery_mint &operator/=(const montgomery_mint &m) {
		val = reduce((uint64_t) val * m.inv().val);
		return *this;
	}

	constexpr friend montgomery_mint operator+(const montgomery_mint &lhs, const montgomery_mint &rhs) {
		return montgomery_mint(lhs) += rhs;
	}

	constexpr friend montgomery_mint operator-(const montgomery_mint &lhs, const montgomery_mint &rhs) {
		return montgomery_mint(lhs) -= rhs;
	}

	constexpr friend montgomery_mint operator*(const montgomery_mint &lhs, const montgomery_mint &rhs) {
		return montgomery_mint(lhs) *= rhs;
	}

	constexpr friend montgomery_mint operator/(const montgomery_mint &lhs, const montgomery_mint &rhs) {
		return montgomery_mint(lhs) /= rhs;
	}

	constexpr friend bool operator==(const montgomery_mint &lhs, const montgomery_mint &rhs) {
		return lhs.val == rhs.val;
	}

	constexpr friend bool operator!=(const montgomery_mint &lhs, const montgomery_mint &rhs) {
		return lhs.val != rhs.val;
	}

	constexpr montgomery_mint &operator++() {
		return *this += 1;
	}

	constexpr montgomery_mint &operator--() {
		return *this -= 1;
	}

	constexpr montgomery_mint operator++(int) {
		montgomery_mint result(*this);
		*this += 1;
		return result;
	}

	constexpr montgomery_mint operator--(int) {
		montgomery_mint result(*this);
		*this -= 1;
		return result;
	}

	template<typename T>
	constexpr explicit operator T() const {
		return T(val);
	}

	friend std::ostream &operator<<(std::ostream &os, const montgomery_mint &m) {
		return os << m.get_val();
	}

	friend std::istream &operator>>(std::istream &is, montgomery_mint &m) {
		long long x;
		return is >> x, m = x, is;
	}
};

template <typename>
struct is_mint_helper : std::false_type { };

template <uint32_t M>
struct is_mint_helper<montgomery_mint<M>> : std::true_type { };

template <typename T>
struct is_mint : is_mint_helper<typename std::decay<T>::type> { };