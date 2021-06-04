#include <cstdint>

template <typename T>
struct promote {
	typedef T type;
};

template <>
struct promote<std::int32_t> {
	typedef std::int64_t type;
};

//template <>
//struct promote<std::int64_t> {
//	typedef __int128_t type;
//};

template <>
struct promote<std::uint32_t> {
	typedef std::uint64_t type;
};

//template <>
//struct promote<std::uint64_t> {
//	typedef __uint128_t type;
//};

template <typename T>
using promote_t = typename promote<T>::type;