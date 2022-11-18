#pragma once

#include <iostream>
#include <algorithm>
#include <numeric>

template <typename T>
struct is_euclidean : std::true_type {};

template <typename T>
using value_t = typename T::value_type;

template <typename EuclideanDomain>
value_t<EuclideanDomain> gcd(const value_t<EuclideanDomain> &x, const value_t<EuclideanDomain> &y) {
    if (x == EuclideanDomain::zero()) {
        return y;
    } else if (y == EuclideanDomain::zero()) {
        return x;
    } else {
        auto r = EuclideanDomain::euclidean_division(x, y).second;
        return gcd(y, r);
    }
}

template <typename IntegralDomain>
struct fraction {
    using integral_domain_type = IntegralDomain;
    using value_type = typename integral_domain_type::value_type;

    value_type a, b;

    fraction(value_type a = integral_domain_type::zero(), value_type b = integral_domain_type::unit()) : a(a), b(b) {
        normalize();
    }

    void normalize() {
        if constexpr (is_euclidean<integral_domain_type>::value) {
            value_type d = gcd(a, b);
            a /= d, b /= d;
            if (b < 0) a *= -1, b *= -1;
        }
    }

    fraction operator+() const {
        return *this;
    }

    fraction operator-() const {
        return fraction(-a, b);
    }

    fraction &operator+=(const fraction &x) {
        std::tie(a, b) = std::make_pair(a * x.b + b * x.a, b * x.b);
        return normalize(), *this;
    }

    fraction &operator-=(const fraction &x) {
        std::tie(a, b) = std::make_pair(a * x.b - b * x.a, b * x.b);
        return normalize(), *this;
    }

    fraction &operator*=(const fraction &x) {
        a *= x.a, b *= x.b;
        return normalize(), *this;
    }

    fraction &operator/=(const fraction &x) {
        a *= x.b, b *= x.a;
        return normalize(), *this;
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
        return os << x.a << '/' << x.b;
    }

    friend bool operator==(const fraction &x, const fraction &y) {
        return x.a == y.a && x.b == y.b;
    }

    friend bool operator!=(const fraction &x, const fraction &y) {
        return !(x == y);
    }

    friend bool operator<(const fraction &x, const fraction &y) {
        return x.a * y.b < x.b * y.a;
    }

    friend bool operator>(const fraction &x, const fraction &y) {
        return x.a * y.b > x.b * y.a;
    }

    friend bool operator<=(const fraction &x, const fraction &y) {
        return !(x > y);
    }

    friend bool operator>=(const fraction &x, const fraction &y) {
        return !(x < y);
    }
};
