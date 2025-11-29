module;

// Include Windows multimedia timer functions for high-resolution sleep
#ifdef _WIN32
extern "C" __declspec(dllimport) unsigned long __stdcall timeBeginPeriod(unsigned long uPeriod);
extern "C" __declspec(dllimport) unsigned long __stdcall timeEndPeriod(unsigned long uPeriod);
#pragma comment(lib, "winmm.lib")
#endif

export module bm.deltatime;

import bm.log;

import std;

namespace bm
{
	export class DeltaTime
	{
	public:
		DeltaTime() :
			m_start(std::chrono::high_resolution_clock::now())
		{
			setFPSLimit(420);
			// Set Windows timer resolution to 1 ms for more accurate sleep
#ifdef _WIN32
			timeBeginPeriod(1);
#endif
		}
		~DeltaTime()
		{
#ifdef _WIN32
			timeEndPeriod(1);
#endif
		}

		void setFPSLimit(unsigned short fps_limit) noexcept
		{
			m_fps_limit = fps_limit;
			m_frame_duration = std::chrono::microseconds((int)(1'000'000.0 / fps_limit));
		}

		void onUpdate() noexcept
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto delta = now - m_start;

			if (delta < m_frame_duration)
				std::this_thread::sleep_for(m_frame_duration - delta);

			now = std::chrono::high_resolution_clock::now();
			delta = now - m_start;
			m_delta_time = std::chrono::duration<float>(delta).count();
			m_start = now;

			m_accumulated_time += m_delta_time;

			if (++m_frame_counter >= 30)
			{
				m_fps = 30.0f / m_accumulated_time;
				m_accumulated_time = 0.0f;
				m_frame_counter = 0;
			}
		}

		float getDeltaTime() const noexcept
		{
			return m_delta_time;
		}

		float getFPS() const noexcept
		{
			return m_fps;
		}

	private:
		std::chrono::high_resolution_clock::time_point m_start;
		std::chrono::microseconds m_frame_duration;
		unsigned short m_fps_limit;
		float m_accumulated_time = 0.0f;
		unsigned int m_frame_counter = 0;
		float m_delta_time = 0.f;
		float m_fps = 0.f;
	};
}