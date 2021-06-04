#include <iostream>
#include <algorithm>

template <typename T>
struct rational {
	static_assert(std::is_integral<T>::value, "Type must be integral");

	T a, b;

	rational(int a = 0, int b = 1) : a(a), b(b) {
		normalize();
	}

	void normalize() {
		T d = std::__gcd(a, b);
		a /= d, b /= d;
		if (b < 0) a *= -1, b *= -1;
	}

	rational operator+() const {
		return *this;
	}

	rational operator-() const {
		return rational(-a, b);
	}

	rational &operator+=(const rational &x) {
		tie(a, b) = make_pair(a * x.b + b * x.a, b * x.b);
		return normalize(), *this;
	}

	rational &operator-=(const rational &x) {
		tie(a, b) = make_pair(a * x.b - b * x.a, b * x.b);
		return normalize(), *this;
	}

	rational &operator*=(const rational &x) {
		a *= x.a, b *= x.b;
		return normalize(), *this;
	}

	rational &operator/=(const rational &x) {
		a *= x.b, b *= x.a;
		return normalize(), *this;
	}

	friend rational operator+(const rational &x, const rational &y) {
		return rational(x) += y;
	}

	friend rational operator-(const rational &x, const rational &y) {
		return rational(x) -= y;
	}

	friend rational operator*(const rational &x, const rational &y) {
		return rational(x) *= y;
	}

	friend rational operator/(const rational &x, const rational &y) {
		return rational(x) /= y;
	}

	friend std::ostream &operator<<(std::ostream &os, const rational &x) {
		return os << x.a << '/' << x.b;
	}

	friend bool operator==(const rational &x, const rational &y) {
		return x.a == y.a && x.b == y.b;
	}

	friend bool operator!=(const rational &x, const rational &y) {
		return !(x == y);
	}

	friend bool operator<(const rational &x, const rational &y) {
		return x.a * y.b < x.b * y.a;
	}

	friend bool operator>(const rational &x, const rational &y) {
		return x.a * y.b > x.b * y.a;
	}

	friend bool operator<=(const rational &x, const rational &y) {
		return !(x > y);
	}

	friend bool operator>=(const rational &x, const rational &y) {
		return !(x < y);
	}
};