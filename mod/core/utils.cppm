export module bm.utility;

import std;

namespace bm
{

	export constexpr unsigned int bit(unsigned int n) { return 1 << n; }
	
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