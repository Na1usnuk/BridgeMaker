export module bm.traits;

import std;

namespace bm
{
	// Why this is not in std yet?
	// This will be here until std implements observer_ptr
	// I dont want to introduce new module for this
	template<typename T>
	class observer_ptr {
	public:
		constexpr observer_ptr() noexcept : m_ptr(nullptr) {}
		constexpr observer_ptr(T* ptr) noexcept : m_ptr(ptr) {}
		constexpr observer_ptr(std::unique_ptr<T> ptr) noexcept : m_ptr(ptr.get()) {}
		template<typename U>
			requires std::is_convertible_v<T*, U*>
		constexpr observer_ptr(const observer_ptr<U>& oth) noexcept : m_ptr(static_cast<T*>(oth.get())) {}

		template<typename U>
			requires std::is_convertible_v<U*, T*>
		constexpr observer_ptr& operator=(const observer_ptr<U>& oth) noexcept
		{
			m_ptr = static_cast<T*>(oth.get());
			return *this;
		}
		constexpr observer_ptr& operator=(T* ptr) noexcept { m_ptr = ptr; return *this; }
		constexpr obserter_ptr& operator=(const std::unique_ptr& ptr) noexcept { m_ptr = ptr.get(); }

		constexpr T* get() const noexcept { return m_ptr; }
		constexpr T& operator*() const noexcept { return *m_ptr; }
		constexpr T* operator->() const noexcept { return m_ptr; }
		constexpr explicit operator bool() const noexcept { return m_ptr != nullptr; }

		constexpr void reset() noexcept { m_ptr = nullptr; }
		constexpr T* release() noexcept { T* p = m_ptr; m_ptr = nullptr; return p; }
	private:
		T* m_ptr;
	};


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
		using SPtrRef = SPtr&;
		using KSPtr = const SPtr;
		using KSPtrRef = KSPtr&;

		using WPtr = std::weak_ptr<T>;
		using WPtrRef = WPtr&;

		using OPtr = observer_ptr<T>;
		using KOPtr = const OPtr;
		using OPtrRef = KOPtr&; // not needed

		using RawPtr = T*;
		using KRawPtr = const T*;

		using Ref = T&;
		using KRef = const Ref;
	};
}