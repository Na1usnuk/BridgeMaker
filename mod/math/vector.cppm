export module bm.math:vector;

import std;

namespace bm::math
{
	template<typename T, std::size_t C>
	class Vector
	{
	public:

		using Type = T;
		using Container = std::array<T, C>;

		static constexpr std::size_t size = C;

	public:

		constexpr Vector() = default;

		constexpr explicit Vector(Type scalar) { m_data.fill(scalar); }

		template<typename... Args>
			requires (sizeof...(Args) == Vector::size and sizeof...(Args) > 0) and (std::is_convertible_v<Args, T> and ...)
		constexpr Vector(Args... args) : m_data{ static_cast<T>(args)... }
		{ }

		constexpr Vector(std::initializer_list<Type> il) 
		{
			std::copy_n(il.begin(), std::min(il.size(), size), m_data.begin());
		}

		constexpr Vector(const Vector&) = default;
		constexpr Vector& operator=(const Vector&) = default;

		constexpr Vector(Vector&&) noexcept = default;
		constexpr Vector& operator=(Vector&&) = default;

		~Vector() = default;

		constexpr Vector& operator+=(const Vector& oth) noexcept
		{
			for (std::size_t i = 0; i < size; ++i) m_data[i] += oth[i];
			return *this;
		}

		constexpr Vector& operator+=(const Type scalar) noexcept
		{
			for (auto& val : m_data) val += scalar;
			return *this;
		}

		constexpr Vector& operator-=(const Vector& oth) noexcept
		{
			for (std::size_t i = 0; i < size; ++i)
				m_data[i] -= oth[i];
			return *this;
		}

		constexpr Vector& operator-=(const Type scalar) noexcept
		{
			return *this += -scalar;
		}

		constexpr Vector& operator*=(const Vector& oth) noexcept
		{
			for (std::size_t i = 0; i < size; ++i) m_data[i] *= oth[i];
			return *this;
		}

		constexpr Vector& operator*=(const Type scalar) noexcept
		{
			for (auto& val : m_data) val *= scalar;
			return *this;
		}

		constexpr Vector& operator/=(const Vector& oth) noexcept
		{
			for (std::size_t i = 0; i < size; ++i) m_data[i] /= oth[i];
			return *this;
		}

		constexpr Vector& operator/=(const T scalar) noexcept
		{
			for (auto& val : m_data) val /= scalar;
			return *this;
		}


		[[nodiscard]] constexpr Type& operator[](std::size_t i) noexcept { return m_data[i]; }
		[[nodiscard]] constexpr const Type& operator[](std::size_t i) const noexcept { return m_data[i]; }

		[[nodiscard]] constexpr static std::size_t getSize() noexcept { return size; }
		[[nodiscard]] constexpr Type* getData() noexcept { return m_data.data(); }
		[[nodiscard]] constexpr const Type* getData() const noexcept { return m_data.data(); }


		[[nodiscard]] constexpr auto begin() noexcept { return m_data.begin(); }
		[[nodiscard]] constexpr auto end() noexcept { return m_data.end(); }

		[[nodiscard]] constexpr auto cbegin() const noexcept { return m_data.cbegin(); }
		[[nodiscard]] constexpr auto cend() const noexcept { return m_data.cend(); }

		[[nodiscard]] constexpr auto rbegin() noexcept { return m_data.rbegin(); }
		[[nodiscard]] constexpr auto rend() noexcept { return m_data.rend(); }

		[[nodiscard]] constexpr auto crbegin() const noexcept { return m_data.crbegin(); }
		[[nodiscard]] constexpr auto crend() const noexcept { return m_data.crend(); }

		[[nodiscard]] constexpr auto begin() const noexcept { return cbegin(); }
		[[nodiscard]] constexpr auto end() const noexcept { return cend(); }

		[[nodiscard]] constexpr auto rbegin() const noexcept { return crbegin(); }
		[[nodiscard]] constexpr auto rend() const noexcept { return crend(); }

	private:

		Container m_data{};
	};


	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator-(const Vector<T, C>& vec)
	{
		Vector<T, C> result(vec);
		for (auto& val : result)
			val = -val;
		return result;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr bool operator==(const Vector<T, C>& lhs, const Vector<T, C>& rhs)
	{
		return std::ranges::equal(lhs, rhs);
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator+(const Vector<T, C>& lhs, const Vector<T, C>& rhs)
	{
		Vector<T, C> result(lhs);
		return result += rhs;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator+(const Vector<T, C>& vec, const T scalar)
	{
		Vector<T, C> result(vec);
		return result += scalar;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator+(T scalar, const Vector<T, C>& vec)
	{
		return vec + scalar;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator-(const Vector<T, C>& lhs, const Vector<T, C>& rhs)
	{
		return lhs + -rhs;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator-(const Vector<T, C>& vec, const T scalar)
	{
		return vec + -scalar;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator-(const T scalar, const Vector<T, C>& vec)
	{
		Vector<T, C> result;
		for (std::size_t i = 0; i < C; ++i)
			result[i] = scalar - vec[i];
		return result;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator*(const Vector<T, C>& lhs, const Vector<T, C>& rhs)
	{
		Vector<T, C> result(lhs);
		return result *= rhs;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator*(const Vector<T, C>& vec, const T scalar)
	{
		Vector<T, C> result(vec);
		return result *= scalar;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> operator*(const T scalar, const Vector<T, C>& vec)
	{
		return vec * scalar;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]] 
		constexpr Vector<T, C> operator/(const Vector<T, C>& lhs, const Vector<T, C>& rhs)
	{
		Vector<T, C> result(lhs);
		return result /= rhs;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]] 
		constexpr Vector<T, C> operator/(const Vector<T, C>& vec, const T scalar)
	{
		Vector<T, C> result(vec);
		return result /= scalar;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]] 
		constexpr Vector<T, C> operator/(const T scalar, const Vector<T, C>& vec)
	{
		Vector<T, C> result;
		for (std::size_t i = 0; i < C; ++i)
			result[i] = scalar / vec[i];
		return result;
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr double computeMagnitudeSquared(const Vector<T, C>& vec)
	{
		return std::accumulate(vec.cbegin(), vec.cend(), 0., [](double acc, T val) { return acc + val * val; });
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr double computeMagnitude(const Vector<T, C>& vec)
	{
		return std::sqrt(computeMagnitudeSquared(vec));
	}

	export template<typename T, std::size_t C>
		[[nodiscard]]
		constexpr Vector<T, C> computeNormalized(const Vector<T, C>& vec)
	{
		const double mag = computeMagnitude(vec);
		if (mag <= std::numeric_limits<double>::epsilon())
			return Vector<T, C>{};

		Vector<T, C> result;
		for (std::size_t i = 0; i < C; ++i)
			result[i] = static_cast<T>(vec[i] / mag);

		return result;
	}

	
	// Float vectors
	export using Vec2 = Vector<float, 2>;
	export using Vec3 = Vector<float, 3>;
	export using Vec4 = Vector<float, 4>;

	// Double precision vectors
	export using DVec2 = Vector<double, 2>;
	export using DVec3 = Vector<double, 3>;
	export using DVec4 = Vector<double, 4>;

	// Integer vectors
	export using IVec2 = Vector<std::int32_t, 2>;
	export using IVec3 = Vector<std::int32_t, 3>;
	export using IVec4 = Vector<std::int32_t, 4>;

	// Unsigned integer vectors
	export using UVec2 = Vector<std::uint32_t, 2>;
	export using UVec3 = Vector<std::uint32_t, 3>;
	export using UVec4 = Vector<std::uint32_t, 4>;

	// Boolean vectors
	//export using BVec2 = Vector<bool, 2>;
	//export using BVec3 = Vector<bool, 3>;
	//export using BVec4 = Vector<bool, 4>;
}