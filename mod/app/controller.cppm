export module bm.app:controller;

import :frame;
import :event;

import std;
import bm.core;
import bm.platform;
import bm.gfx;


namespace bm::app
{
	export class DebugCameraController
	{
	public:

		DebugCameraController(gfx::Camera& camera_to_control, const platform::Window& window, float speed = 3.0f, float sensitivity = .5f);

		void onUpdate(DeltaTime delta_time);
		void onEvent(Event& e);

		bool onMouseMove(MouseMoveEvent& e);
		bool onScroll(MouseScrollEvent& e);
		bool onMouseClick(MouseButtonPressEvent& e);
		bool onMouseRelease(MouseButtonReleaseEvent& e);

		void setSpeed(float speed) noexcept { m_speed = speed; }
		float getSpeed() const noexcept { return m_speed; }
		void setSensitivity(float sensitivity) noexcept { m_sensitivity = sensitivity; }
		float getSensetivity() noexcept { return m_sensitivity; }

	private:

		gfx::Camera* m_camera;
		platform::Input m_input;

		float m_speed;
		float m_sensitivity;

		std::pair<float, float> m_last_mouse_pos;

		bool m_first_mouse_event = true;
		bool m_camera_active = false;

	};
}