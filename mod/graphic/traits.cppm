module;
#include "bmpch.hpp"
export module bm.traits;


namespace bm
{
	export template<typename T>
	struct Traits
	{
		using Ptr = std::shared_ptr<T>;
		using WPrt = std::weak_ptr<T>;
		using KPtr = const Ptr;
		using KPtrRef = KPtr&;
	};
}