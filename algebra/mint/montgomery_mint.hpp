#pragma once

#include "concepts.hpp"
#include <cstdint>
#include <iostream>

namespace algebra {

template <uint32_t M>
struct montgomery_mint {
    static_assert(0 < M, "Modulus must be non-zero");
    static_assert(M % 2 != 0, "Modulus must be odd");
    static_assert(M < (1u << 31), "Modulus must fit in a 32-bit signed integer");

    constexpr static auto get_m() {
        return M;
    }

    constexpr static auto get_r() {
        return -M % M;
    }

    constexpr static auto get_r_inverse() {
        constexpr auto r = get_r();
        uint32_t r_inverse = 1, r_pow = r;
        for (uint32_t n = M - 2; n != 0; n /= 2) {
            if (n % 2 != 0) {
                r_inverse = (uint64_t)r_inverse * r_pow % M;
            }
            r_pow = (uint64_t)r_pow * r_pow % M;
        }
        return r_inverse;
    }

    constexpr static auto get_r_squared() {
        return (uint64_t)get_r() * get_r() % M;
    }

    constexpr static auto get_m_dash() {
        constexpr auto r_inverse = get_r_inverse();
        return (uint32_t)((((uint64_t)r_inverse << 32) - 1) / M);
    }

    constexpr static uint32_t reduce(uint64_t x) {
        constexpr auto m_dash = get_m_dash();
        uint32_t y = (x + (uint64_t)((uint32_t)x * m_dash) * M) >> 32;
        return y >= M ? y - M : y;
    }

    uint32_t val;

    constexpr uint32_t get_val() const {
        return reduce(val);
    }

    constexpr montgomery_mint() : val() {}

    constexpr montgomery_mint(long long x) : val() {
        constexpr auto r_squared = get_r_squared();
        x %= (int)M;
        val = reduce((x < 0 ? x + M : x) * r_squared);
    }

    constexpr montgomery_mint pow(long long n) const {
        montgomery_mint ans = 1, x(*this);
        for (; n > 0; n /= 2) {
            if (n & 1)
                ans *= x;
            x *= x;
        }
        return ans;
    }

    constexpr montgomery_mint inv() const {
        return pow(M - 2);
    }

    constexpr montgomery_mint operator+() const {
        montgomery_mint m;
        m.val = val;
        return m;
    }

    constexpr montgomery_mint operator-() const {
        montgomery_mint m;
        m.val = (val == 0 ? 0 : M - val);
        return m;
    }

    constexpr montgomery_mint &operator+=(const montgomery_mint &m) {
        if ((val += m.val) >= M)
            val -= M;
        return *this;
    }

    constexpr montgomery_mint &operator-=(const montgomery_mint &m) {
        if ((val -= m.val) >= M)
            val += M;
        return *this;
    }

    constexpr montgomery_mint &operator*=(const montgomery_mint &m) {
        val = reduce((uint64_t)val * m.val);
        return *this;
    }

    constexpr montgomery_mint &operator/=(const montgomery_mint &m) {
        val = reduce((uint64_t)val * m.inv().val);
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

    template <typename T>
    constexpr explicit operator T() const {
        return T(get_val());
    }

    friend std::ostream &operator<<(std::ostream &os, const montgomery_mint &m) {
        return os << m.get_val();
    }

    friend std::istream &operator>>(std::istream &is, montgomery_mint &m) {
        long long x;
        return is >> x, m = x, is;
    }
};

template <uint32_t M>
struct _detail::is_static_mint_helper<montgomery_mint<M>> : std::true_type {};

} // namespace algebra
