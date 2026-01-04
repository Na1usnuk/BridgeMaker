export module bm.core:utility;

import std;

namespace bm
{

	export constexpr unsigned int bit(unsigned int n) { return 1 << n; }
	
	// A non-owning smart(dumb) pointer
	export template<typename T>
	class observer_ptr {
	public:
		constexpr observer_ptr() noexcept : m_ptr(nullptr) {}
		constexpr observer_ptr(T* ptr) noexcept : m_ptr(ptr) {}
		constexpr observer_ptr(const std::unique_ptr<T>& ptr) noexcept : m_ptr(ptr.get()) {}
		template<typename U>
			requires std::is_convertible_v<U*, T*>
		constexpr observer_ptr(const observer_ptr<U>& oth) noexcept : m_ptr(static_cast<T*>(oth.get())) {}

		template<typename U>
			requires std::is_convertible_v<U*, T*>
		constexpr observer_ptr& operator=(const observer_ptr<U>& oth) noexcept
		{
			m_ptr = static_cast<T*>(oth.get());
			return *this;
		}
		constexpr observer_ptr& operator=(T* ptr) noexcept { m_ptr = ptr; return *this; }
		constexpr observer_ptr& operator=(const std::unique_ptr<T>& ptr) noexcept { m_ptr = ptr.get(); return *this; }

		constexpr bool operator==(std::nullptr_t) const noexcept { return m_ptr == nullptr; }
		constexpr bool operator!=(std::nullptr_t) const noexcept { return m_ptr != nullptr; }

		constexpr T* get() noexcept { return m_ptr; }
		constexpr const T* get() const noexcept { return m_ptr; }

		constexpr T& operator*() noexcept { return *m_ptr; }
		constexpr const T& operator*() const noexcept { return *m_ptr; }

		constexpr T* operator->() noexcept { return m_ptr; }
		constexpr const T* operator->() const noexcept { return m_ptr; }

		constexpr explicit operator bool() const noexcept { return m_ptr != nullptr; }

		constexpr void reset() noexcept { m_ptr = nullptr; }
	private:
		T* m_ptr;
	};


	// Default exception
	export struct Exception : public std::runtime_error
	{
		Exception(std::string_view what) : std::runtime_error(what.data()) {}
	};

}