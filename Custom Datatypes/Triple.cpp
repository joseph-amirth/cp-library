template<typename T1, typename T2, typename T3>
struct triple {
	T1 first;
	T2 second;
	T3 third;

	triple() : first(), second(), third() {}
	triple(T1 first, T2 second, T3 third) : first(first), second(second), third(third) {}

	bool operator==(const triple &rhs) const { return tie(first, second, third) == tie(rhs.first, rhs.second, rhs.third); }
	bool operator!=(const triple &rhs) const { return !(*this == rhs); }
	bool operator<(const triple &rhs) const { return tie(first, second, third) < tie(rhs.first, rhs.second, rhs.third); }
	bool operator>(const triple &rhs) const { return rhs < *this; }
	bool operator<=(const triple &rhs) const { return !(*this > rhs); }
	bool operator>=(const triple &rhs) const { return !(*this < rhs); }
};