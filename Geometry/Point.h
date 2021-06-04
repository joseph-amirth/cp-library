template <typename T>
struct point {
	T x, y;
	point() : x(), y() { }
	point(T x, T y) : x(x), y(y) { }

	point operator+() const { return point(x, y); }
	point operator-() const { return point(-x, -y); }

	point &operator+=(const point &p) { return x += p.x, y += p.y, *this; }
	point &operator-=(const point &p) { return x -= p.x, y -= p.y, *this; }
	point &operator*=(const T &val) { return x *= val, y *= val, *this; }
	point &operator/=(const T &val) { return x /= val, y /= val, *this; }

	point operator+(const point &p) const { return point(x + p.x, y + p.y); }
	point operator-(const point &p) const { return point(x - p.x, y - p.y); }
	point operator*(const T &val) const { return point(x * val, y * val); }
	point operator/(const T &val) const { return point(x / val, y / val); }

	friend point operator*(const T &val, const point &p) {
		return point(val * p.x, val * p.y);
	}

	bool operator==(const point &p) const { return x == p.x && y == p.y; }
	bool operator!=(const point &p) const { return x != p.x || y != p.y; }
	bool operator< (const point &p) const { return x < p.x || (x == p.x && y < p.y); }
	bool operator> (const point &p) const { return x > p.x || (x == p.x && y > p.y); }
	bool operator<=(const point &p) const { return !(*this > p); }
	bool operator>=(const point &p) const { return !(*this < p); }

	template <typename U> operator point<U>() {
		return point<U>(x, y);
	}

	friend T dot(const point &a, const point &b) {
		return a.x * b.x + a.y * b.y;
	}
};