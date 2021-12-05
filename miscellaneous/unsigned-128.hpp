#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

struct uint128_t {
    uint64_t a, b;

    uint128_t() : a(0), b(0) {}

    uint128_t(int64_t x) : a(0), b(x) {
        if (x < 0) {
            b = -x;
            *this = -(*this);
        }
    }

    uint128_t(uint64_t a, uint64_t b) : a(a), b(b) {}

    uint128_t(const string &s) : a(0), b(0) {
        assert(!s.empty());
        bool sign = s.front() == '-';
        uint128_t ten = 1;
        for (int i = (int) s.length() - 1; i >= sign; i--) {
            assert(isdigit(s[i]));
            *this += ten * (s[i] - '0'), ten *= 10;
        }
        if (sign) {
            *this = -(*this);
        }
    }

    uint128_t operator~() const { return uint128_t(~a, ~b); }

    uint128_t operator-() const { return ~(*this) + 1; }

    uint128_t operator+() const { return *this; }

    uint128_t &operator|=(const uint128_t &x) {
        a |= x.a;
        b |= x.b;
        return *this;
    }

    uint128_t operator|(const uint128_t &x) const {
        uint128_t result = *this;
        return result |= x;
    }

    uint128_t &operator&=(const uint128_t &x) {
        a &= x.a;
        b &= x.b;
        return *this;
    }

    uint128_t operator&(const uint128_t &x) const {
        uint128_t result = *this;
        return result &= x;
    }

    uint128_t &operator^=(const uint128_t &x) {
        a ^= x.a;
        b ^= x.b;
        return *this;
    }

    uint128_t operator^(const uint128_t &x) const {
        uint128_t result = *this;
        return result ^= x;
    }

    uint128_t &operator<<=(uint64_t x) {
        assert(x >= 0);
        if (x >= 128) {
            a = 0, b = 0;
        } else if (x >= 64) {
            a = (b << (x - 64));
            b = 0;
        } else if (x > 0) {
            a = (a << x) | (b >> (64 - x));
            b <<= x;
        }
        return *this;
    }

    uint128_t operator<<(uint64_t x) const {
        uint128_t result = *this;
        return result <<= x;
    }

    uint128_t &operator>>=(uint64_t x) {
        assert(x >= 0);
        if (x >= 128) {
            a = 0, b = 0;
        } else if (x >= 64) {
            b = (a >> (x - 64));
            a = 0;
        } else if (x > 0) {
            b = (b >> x) | (a << (64 - x));
            a >>= x;
        }
        return *this;
    }

    uint128_t operator>>(uint64_t x) const {
        uint128_t result = *this;
        return result >>= x;
    }

    uint128_t &operator+=(const uint128_t &x) {
        if (x.b > ULLONG_MAX - b) {
            a += x.a + 1;
        }
        b += x.b;
        return *this;
    }

    uint128_t operator+(const uint128_t &x) const {
        uint128_t result = *this;
        return result += x;
    }

    uint128_t &operator-=(const uint128_t &x) { return *this += (-x); }

    uint128_t operator-(const uint128_t &x) const {
        uint128_t result = *this;
        return result -= x;
    }

    uint128_t multiply(uint64_t x, uint64_t y) const {
        uint64_t a1 = (x >> 32), a2 = (y >> 32);
        uint64_t b1 = x ^ (a1 << 32), b2 = y ^ (a2 << 32);
        return (uint128_t(a1 * a2) << 64) + (uint128_t(a1 * b2) + uint128_t(a2 * b1) << 32) + uint128_t(b1 * b2);
    }

    uint128_t &operator*=(const uint128_t &x) {
        return *this = (multiply(a, x.b) + multiply(x.a, b) << 64) + multiply(b, x.b);
    }

    uint128_t operator*(const uint128_t &x) const {
        return (multiply(a, x.b) + multiply(x.a, b) << 64) + multiply(b, x.b);
    }

    pair<uint128_t, uint128_t> div_mod(const uint128_t &d) const {
        assert(d != 0);
        uint128_t q, r;
        for (int i = 127; i >= 0; i--) {
            r <<= 1, r |= is_set(i);
            if (r >= d) r -= d, q |= uint128_t(1) << i;
        }
        return make_pair(q, r);
    }

    uint128_t &operator/=(const uint128_t &x) { return *this = div_mod(x).first; }

    uint128_t operator/(const uint128_t &x) const { return div_mod(x).first; }

    uint128_t &operator%=(const uint128_t &x) { return *this = div_mod(x).second; }

    uint128_t operator%(const uint128_t &x) const { return div_mod(x).second; }

    bool operator==(const uint128_t &x) const { return a == x.a && b == x.b; }

    bool operator!=(const uint128_t &x) const { return !(x == *this); }

    bool operator<(const uint128_t &x) const { return (a < x.a || (a == x.a && b < x.b)); }

    bool operator>(const uint128_t &x) const { return x < *this; }

    bool operator<=(const uint128_t &x) const { return !(x < *this); }

    bool operator>=(const uint128_t &x) const { return !(*this < x); }

    explicit operator bool() {
        return a || b;
    }

    bool is_set(int i) const {
        if (i < 0 || 127 < i) return false;
        if (i >= 64) return a & (1ull << (i - 64));
        return b & (1ull << i);
    }

    int count() const {
        return __builtin_popcountll(a) + __builtin_popcountll(b);
    }

    int clz() const {
        return a ? __builtin_clzll(a) : 64 + __builtin_clzll(b);
    }

    int ctz() const {
        return b ? __builtin_ctzll(b) : __builtin_ctzll(a) + 64;
    }

    string to_string() const {
        string s;
        uint128_t x = *this;
        if (x == 0) {
            x += '0';
        } else {
            while (x) {
                auto[q, r] = x.div_mod(10);
                s += '0' + r.b;
                x = q;
            }
            for (int i = 0; i < (int) s.length() / 2; i++) {
                swap(*(s.begin() + i), *(s.rbegin() + i));
            }
        }
        return s;
    }

    friend ostream &operator<<(ostream &os, const uint128_t &x) {
        os << x.to_string();
        return os;
    }

    friend istream &operator>>(istream &is, uint128_t &x) {
        string str;
        is >> str;
        x = uint128_t(str);
        return is;
    }
};