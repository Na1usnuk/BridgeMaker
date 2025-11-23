module;

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
			m_start(std::chrono::high_resolution_clock::now()),
			m_frame_duration(std::chrono::microseconds(1)),
			m_fps_limit(998),// It breaks on 999 because of integer division
			m_delta_time(0.0f),
			m_fps(0.0f)
		{
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
			if (fps_limit == m_fps_limit) return;
			m_fps_limit = fps_limit;
			m_frame_duration = std::chrono::microseconds((int)(1'000'000.0 / fps_limit));
		}

		void onUpdate() noexcept
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto delta = now - m_start;

			if (delta < m_frame_duration)
				std::this_thread::sleep_for(m_frame_duration - delta);

			m_delta_time = std::chrono::duration<float>(m_frame_duration).count();
			m_start += m_frame_duration;

			// FPS counter - average over 30 frames
			static int frame_counter = 0;
			static float thirty_deltas = 0.0f;
			thirty_deltas += m_delta_time;

			if (++frame_counter >= 30)
			{
				m_fps = 30.0f / thirty_deltas;
				thirty_deltas = 0.0f;
				frame_counter = 0;
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
		float m_delta_time;
		float m_fps;
	};
}