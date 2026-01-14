export module bm.core:time;

import :verify;

import std;

namespace bm
{

	// Helper concepts for chrono types
	template<typename T>
	concept ChronoRatio = requires 
	{
		typename std::ratio<T::num, T::den>;
		requires std::same_as<T, std::ratio<T::num, T::den>>;
	};

	template<typename T>
	concept ChronoDuration = requires 
	{
		typename T::rep;
		typename T::period;
		requires std::same_as<T, std::chrono::duration<typename T::rep, typename T::period>>;
	};

	// Flexible Timer class
	export template<ChronoRatio Precision = std::nano, typename Representation = std::uint64_t>
		requires std::is_arithmetic_v<Representation>
	class Timer final
	{
	public:

		using Duration = std::chrono::duration<Representation, Precision>;
		using Clock = std::chrono::steady_clock;
		using TimePoint = Clock::time_point;

	public:

		void start() noexcept
		{
			m_start = Clock::now();
		}

		// Stop the timer and get elapsed time in specified duration type (default to timer precision)
		template<ChronoDuration R = Duration>
			requires std::is_convertible_v<Duration, R>
		[[nodiscard]]
		R stop() noexcept
		{
			core::verify(m_start != TimePoint{}, "Timer was not started");
			const auto elapsed = std::chrono::duration_cast<R>(Clock::now() - m_start);
			reset();
			return elapsed;
		}

		// Stop the timer and get elapsed time in specified floating type and specified precision (default to seconds)
		template<std::floating_point R, ChronoDuration T = std::chrono::seconds>
		[[nodiscard]]
		R stop() noexcept
		{
			core::verify(m_start != TimePoint{}, "Timer was not started");
			const auto elapsed = std::chrono::duration<R, typename T::period>(Clock::now() - m_start).count();
			reset();
			return elapsed;
		}

		// Restart the timer and get elapsed time in specified duration type (default to timer precision)
		template<ChronoDuration R = Duration>
			requires std::is_convertible_v<Duration, R>
		[[nodiscard]]
		R restart() noexcept
		{
			core::verify(m_start != TimePoint{}, "Timer was not started");
			const auto now = Clock::now();
			const auto elapsed = std::chrono::duration_cast<R>(now - m_start);
			m_start = now;
			return elapsed;
		}

		// Restart the timer and get elapsed time in specified floating type and specified precision (default to seconds)
		template<std::floating_point R, ChronoDuration T = std::chrono::seconds>
		[[nodiscard]]
		R restart() noexcept
		{
			core::verify(m_start != TimePoint{}, "Timer was not started");
			const auto now = Clock::now();
			const auto elapsed = std::chrono::duration<R, typename T::period>(now - m_start).count();
			m_start = now;
			return elapsed;
		}

		void reset() noexcept
		{
			m_start = TimePoint{};
		}

		[[nodiscard]] 
		bool isRunning() const noexcept
		{
			return m_start != TimePoint{};
		}

		// Get elapsed time in specified duration type (default to timer precision)
		template<ChronoDuration R = Duration>
			requires std::is_convertible_v<Duration, R>
		[[nodiscard]]
		R getElapsed() const noexcept
		{
			core::verify(m_start != TimePoint{}, "Timer was not started");
			return std::chrono::duration_cast<R>(Clock::now() - m_start);
		}

		// Get elapsed time as floating point in specified duration type (default to seconds)
		template<std::floating_point R, ChronoDuration T = std::chrono::seconds>
		[[nodiscard]]
		R getElapsed() const noexcept
		{
			core::verify(m_start != TimePoint{}, "Timer was not started");
			return std::chrono::duration<R, typename T::period>(Clock::now() - m_start).count();
		}

	private:

		TimePoint m_start{};
	};

}