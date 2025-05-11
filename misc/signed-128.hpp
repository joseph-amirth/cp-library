#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

struct int128_t {
    uint64_t a, b;

    int128_t() : a(0), b(0) {}

    int128_t(int64_t x) : a(0), b(x) {
        if (x < 0) {
            b = -x;
            *this = -(*this);
        }
    }

    int128_t(uint64_t a, uint64_t b) : a(a), b(b) {}

    int128_t(const string &s) : a(0), b(0) {
        assert(!s.empty());
        bool sign = s.front() == '-';
        int128_t ten = 1;
        for (int i = (int) s.length() - 1; i >= sign; i--) {
            assert(isdigit(s[i]));
            *this += ten * (s[i] - '0'), ten *= 10;
        }
        if (sign) {
            *this = -(*this);
        }
    }

    int128_t operator~() const { return int128_t(~a, ~b); }

    int128_t operator-() const { return ~(*this) + 1; }

    int128_t operator+() const { return *this; }

    int128_t &operator|=(const int128_t &x) {
        a |= x.a;
        b |= x.b;
        return *this;
    }

    int128_t operator|(const int128_t &x) const {
        int128_t result = *this;
        return result |= x;
    }

    int128_t &operator&=(const int128_t &x) {
        a &= x.a;
        b &= x.b;
        return *this;
    }

    int128_t operator&(const int128_t &x) const {
        int128_t result = *this;
        return result &= x;
    }

    int128_t &operator^=(const int128_t &x) {
        a ^= x.a;
        b ^= x.b;
        return *this;
    }

    int128_t operator^(const int128_t &x) const {
        int128_t result = *this;
        return result ^= x;
    }

    int128_t &operator<<=(uint64_t bits) {
        if (bits >= 128) {
            a = 0, b = 0;
        } else if (bits >= 64) {
            a = (b << (bits - 64));
            b = 0;
        } else if (bits > 0) {
            a = (a << bits) | (b >> (64 - bits));
            b <<= bits;
        }
        return *this;
    }

    friend int128_t operator<<(const int128_t &x, uint64_t bits) {
        return int128_t(x) <<= bits;
    }

    int128_t &operator>>=(uint64_t x) {
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

    friend int128_t operator>>(const int128_t &x, uint64_t bits) {
        return int128_t(x) >>= bits;
    }

    int128_t &operator+=(const int128_t &x) {
        if (x.b > ULLONG_MAX - b) {
            a += x.a + 1;
        }
        b += x.b;
        return *this;
    }

    int128_t operator+(const int128_t &x) const {
        int128_t result = *this;
        return result += x;
    }

    int128_t &operator-=(const int128_t &x) { return *this += (-x); }

    int128_t operator-(const int128_t &x) const {
        int128_t result = *this;
        return result -= x;
    }

    int128_t multiply(uint64_t x, uint64_t y) const {
        uint64_t a1 = (x >> 32), a2 = (y >> 32);
        uint64_t b1 = x ^ (a1 << 32), b2 = y ^ (a2 << 32);
        return (int128_t(a1 * a2) << 64) + (int128_t(a1 * b2) + int128_t(a2 * b1) << 32) + int128_t(b1 * b2);
    }

    int128_t &operator*=(const int128_t &x) {
        return *this = (multiply(a, x.b) + multiply(x.a, b) << 64) + multiply(b, x.b);
    }

    int128_t operator*(const int128_t &x) const {
        return (multiply(a, x.b) + multiply(x.a, b) << 64) + multiply(b, x.b);
    }

    pair<int128_t, int128_t> div_mod(int128_t d) const {
        assert(d != 0);
        int128_t n = *this;

        bool sign = (n.a & (1ull << 63)) ^ (d.a & (1ull << 63));
        if (n < 0) n = -n;
        if (d < 0) d = -d;
        int128_t q, r;
        for (int i = 127; i >= 0; i--) {
            r <<= 1, r |= n.is_set(i);
            if (r >= d) {
                r -= d;
                q |= int128_t(1) << i;
            }
        }
        if (sign) {
            q = -q;
            if (r != 0) r = d - r;
        }
        return make_pair(q, r);
    }

    int128_t &operator/=(const int128_t &x) { return *this = div_mod(x).first; }

    int128_t operator/(const int128_t &x) const { return div_mod(x).first; }

    int128_t &operator%=(const int128_t &x) { return *this = div_mod(x).second; }

    int128_t operator%(const int128_t &x) const { return div_mod(x).second; }

    bool operator==(const int128_t &x) const { return a == x.a && b == x.b; }

    bool operator!=(const int128_t &x) const { return !(x == *this); }

    bool operator<(const int128_t &x) const {
        if ((a & (1ull << 63)) != (x.a & (1ull << 63)))
            return a & (1ull << 63);
        if (a & (1ull << 63))
            return a > x.a || (a == x.a && b > x.b);
        return a < x.a || (a == x.a && b < x.b);
    }

    bool operator>(const int128_t &x) const { return x < *this; }

    bool operator<=(const int128_t &x) const { return !(x < *this); }

    bool operator>=(const int128_t &x) const { return !(*this < x); }

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
        int128_t x = *this;
        if (x == 0) {
            x += '0';
        } else {
            bool sign = x < 0;
            if (sign) {
                x = -x;
            }
            while (x) {
                auto[q, r] = x.div_mod(10);
                s += '0' + r.b;
                x = q;
            }
            if (sign) {
                s += '-';
            }
            for (int i = 0; i < (int) s.length() / 2; i++) {
                swap(*(s.begin() + i), *(s.rbegin() + i));
            }
        }
        return s;
    }

    friend ostream &operator<<(ostream &os, const int128_t &x) {
        os << x.to_string();
        return os;
    }

    friend istream &operator>>(istream &is, int128_t &x) {
        string str;
        is >> str;
        x = int128_t(str);
        return is;
    }
};