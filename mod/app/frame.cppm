export module bm.app:frame;

import std;

import bm.core;

namespace bm::app
{
	export using DeltaTime = std::chrono::duration<float>; // Seconds as float

	export class FrameTimer
	{
	public:

		FrameTimer(unsigned int target_fps) noexcept :
			m_target_fps(target_fps),
			m_target_frame_duration(std::chrono::nanoseconds(1'000'000'000 / target_fps))
		{ }

		void startFrame()
		{
			m_timer.start();
		}

		void endFrame()
		{
			const auto elapsed = m_timer.getElapsed<std::chrono::nanoseconds>();

			if (elapsed < m_target_frame_duration)
				std::this_thread::sleep_for(m_target_frame_duration - elapsed);

			m_frame_duration = m_timer.stop<DeltaTime>();
		}

		DeltaTime getDeltaTime() const noexcept
		{
			return m_frame_duration;
		}

		std::chrono::nanoseconds getFrameDuration() const noexcept
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(m_frame_duration);
		}

		void setTargetFPS(unsigned int target_fps) noexcept
		{
			m_target_fps = target_fps;
			m_target_frame_duration = std::chrono::nanoseconds(1'000'000'000 / target_fps);
		}

	private:

		core::Timer<> m_timer;

		unsigned int m_target_fps;
		std::chrono::nanoseconds m_target_frame_duration;
		DeltaTime m_frame_duration{};
	};
}