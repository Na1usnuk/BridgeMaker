export module bm.core:utility;

import std;

namespace bm
{

	export constexpr unsigned int bit(unsigned int n) { return 1 << n; }

	export inline void debugBreak()
	{
		#if defined(_MSC_VER)
			__debugbreak();
		#elif defined(__clang__)
			__builtin_debugtrap();
		#elif defined(__GNUC__)
			__builtin_trap();
		#else
			*(volatile int*)0 = 0;
		#endif
	}
	
	// Some useful concepts
	export template<typename T>
	concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;
	
	export template<typename T>
	concept Container = requires (T c)
	{
		{ std::data(c) } -> std::convertible_to<const void*>;
		{ c.size() } -> std::convertible_to<std::size_t>;
		typename T::value_type;
	};
	
	export template<typename T>
	concept PtrToData = std::is_convertible_v<T, const void*>;

}