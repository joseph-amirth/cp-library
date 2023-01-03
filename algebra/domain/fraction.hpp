#pragma once

#include <iostream>
#include <algorithm>
#include <numeric>

namespace algebra {

template <typename Integer>
struct fraction {
    using value_type = Integer;

    value_type num, den;

    fraction(value_type num = value_type(0), value_type den = value_type(1)) : num(num), den(den) {
        reduce();
        if (den < 0) {
            num *= -1, den *= -1;
        }
    }

    void reduce() {
        value_type g = std::gcd(num, den);
        num /= g, den /= g;
    }

    fraction operator+() const {
        return *this;
    }

    fraction operator-() const {
        return fraction(-num, den);
    }

    fraction &operator+=(const fraction &x) {
        std::tie(num, den) = std::make_pair(num * x.den + den * x.num, den * x.den);
        return reduce(), *this;
    }

    fraction &operator-=(const fraction &x) {
        std::tie(num, den) = std::make_pair(num * x.den - den * x.num, den * x.den);
        return reduce(), *this;
    }

    fraction &operator*=(const fraction &x) {
        num *= x.num, den *= x.den;
        return reduce(), *this;
    }

    fraction &operator/=(const fraction &x) {
        num *= x.den, den *= x.num;
        return reduce(), *this;
    }

    friend fraction operator+(const fraction &x, const fraction &y) {
        return fraction(x) += y;
    }

    friend fraction operator-(const fraction &x, const fraction &y) {
        return fraction(x) -= y;
    }

    friend fraction operator*(const fraction &x, const fraction &y) {
        return fraction(x) *= y;
    }

    friend fraction operator/(const fraction &x, const fraction &y) {
        return fraction(x) /= y;
    }

    friend std::ostream &operator<<(std::ostream &os, const fraction &x) {
        return os << x.num << '/' << x.den;
    }

    friend bool operator==(const fraction &x, const fraction &y) {
        return x.num == y.num && x.den == y.den;
    }

    friend bool operator!=(const fraction &x, const fraction &y) {
        return !(x == y);
    }

    friend bool operator<(const fraction &x, const fraction &y) {
        return x.num * y.den < x.den * y.num;
    }

    friend bool operator>(const fraction &x, const fraction &y) {
        return x.num * y.den > x.den * y.num;
    }

    friend bool operator<=(const fraction &x, const fraction &y) {
        return !(x > y);
    }

    friend bool operator>=(const fraction &x, const fraction &y) {
        return !(x < y);
    }
};

}
