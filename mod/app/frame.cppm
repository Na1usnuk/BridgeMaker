export module bm.app:frame;

import std;

import bm.core;

namespace bm::app
{
	export using DeltaTime = std::chrono::milliseconds;

	export class FrameTimer
	{
	public:

		FrameTimer(unsigned int target_fps) noexcept :
			m_target_fps(target_fps),
			m_target_frame_duration(static_cast<long long>((1000. / static_cast<double>(target_fps)) * 1000))
		{ }

		void startFrame()
		{
			m_timer.start();
		}

		void endFrame()
		{
			const auto end = m_timer.getElapsed();
			if (end < m_target_frame_duration)
				std::this_thread::sleep_for(m_target_frame_duration - end);
			m_frame_duration = m_timer.stop<std::chrono::milliseconds>();
		}

		DeltaTime getDeltaTime() const noexcept
		{
			return m_timer.getElapsed<std::chrono::milliseconds>();
		}

		std::chrono::milliseconds getFrameDuration() const noexcept
		{
			return m_frame_duration;
		}

		void setTargetFPS(unsigned int target_fps) noexcept
		{
			m_target_fps = target_fps;
		}

	private:

		core::Timer<> m_timer;

		unsigned int m_target_fps;
		std::chrono::nanoseconds m_target_frame_duration;
		std::chrono::milliseconds m_frame_duration{};
	};
}