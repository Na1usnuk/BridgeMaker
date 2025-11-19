export module bm.traits;

import std;

namespace bm
{
	// K prefix = Konstant (const)
	// Ptr = std::unique_ptr (default ownership)
	// SPtr = std::shared_ptr (shared ownership)
	// WPtr = std::weak_ptr
	export template<typename T>
	struct Traits
	{
		using Ptr = std::unique_ptr<T>;
		using KPtr = const Ptr;
		using KPtrRef = KPtr&;

		using SPtr = std::shared_ptr<T>;
		using KSPtr = const SPtr;
		using KSPtrRef = KSPtr&;

		using WPtr = std::weak_ptr<T>;

		using RawPtr = T*;
		using KRawPtr = const T*;

		using Ref = T&;
		using KRef = const Ref;
	};
}