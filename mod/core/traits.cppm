export module bm.core:traits;

import :utility;

import std;

namespace bm
{

	// K prefix = Konstant (const)
	// Ptr = std::unique_ptr (default ownership)
	// SPtr = std::shared_ptr (shared ownership)
	// WPtr = std::weak_ptr
	// OPtr = bm::observer_ptr (thin raw pointer wrapper)
	export template<typename T>
	struct Traits
	{
		using Ptr = std::unique_ptr<T>;
		using KPtr = const Ptr;
		using PtrRef = Ptr&;
		using KPtrRef = KPtr&;

		using SPtr = std::shared_ptr<T>;
		using SPtrRef = SPtr&;
		using KSPtr = const SPtr;
		using KSPtrRef = KSPtr&;

		using WPtr = std::weak_ptr<T>;
		using WPtrRef = WPtr&;

		using OPtr = observer_ptr<T>;
		using KOPtr = const OPtr;

		using RawPtr = T*;
		using KRawPtr = const T*;

		using Ref = T&;
		using KRef = const Ref;
	};
}