export module bm.deltatime;

import bm.log;

import std;

namespace bm
{
	export class DeltaTime
	{
	public:

		DeltaTime()
			: m_fps_limit(1000),
			  m_frame_duration(std::chrono::milliseconds(1)),
			  m_last_frame(std::chrono::high_resolution_clock::now()),
			  m_start(std::chrono::high_resolution_clock::now()),
			  m_delta_time(0.0f)
		{
		}

		void setFPSLimit(unsigned short fps_limit)
		{
			if (fps_limit == m_fps_limit) return;
			m_fps_limit = fps_limit;
			m_frame_duration = std::chrono::milliseconds(1000 / fps_limit);
		}

		void update()
		{
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> delta = now - m_last_frame;
			m_delta_time = delta.count();
			m_last_frame = now;
			m_start = now;
		}

		void wait()
		{
			auto delta = std::chrono::high_resolution_clock::now() - m_start;
			if (delta < m_frame_duration)
				std::this_thread::sleep_for(m_frame_duration - delta);
		}

		float getDeltaTime()
		{
			return m_delta_time;
		}


	private:

		std::chrono::high_resolution_clock::time_point m_start;
		std::chrono::high_resolution_clock::time_point m_last_frame;
		std::chrono::milliseconds m_frame_duration;

		float m_delta_time;
		unsigned short m_fps_limit;

	};
}