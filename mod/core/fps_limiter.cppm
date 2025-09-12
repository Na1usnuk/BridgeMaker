export module bm.fps_limiter;

import bm.log;

import std;

namespace bm
{
	export class FPSLimiter
	{
	public:

		FPSLimiter(unsigned short fps_limit = 1000) : m_start(std::chrono::high_resolution_clock::now()), m_frame_duration(1000 / fps_limit)
		{
		}

		~FPSLimiter()
		{
			auto delta = std::chrono::high_resolution_clock::now() - m_start;
			if (delta < m_frame_duration)
				std::this_thread::sleep_for(m_frame_duration - delta);
		}

	private:

		std::chrono::high_resolution_clock::time_point m_start;
		std::chrono::milliseconds m_frame_duration;

	};
}