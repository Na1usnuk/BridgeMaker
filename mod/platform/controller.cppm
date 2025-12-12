export module bm.platform:controller;

import std;
import bm.core;

import :input;

namespace bm
{
	export class DebugCameraController : public Layer
	{
	public:

		DebugCameraController(Traits<Camera>::OPtr camera_to_control, float speed = 3.0f, float sensetivity = .5f);
		DebugCameraController(Camera& camera_to_control, float speed = 3.0f, float sensetivity = .5f) : DebugCameraController(&camera_to_control, speed, sensetivity) {}

		void onUpdate(float delta_time) override;
		void onEvent(event::Event& e) override;

		bool onMouseMove(event::MouseMove& e);
		bool onScroll(event::MouseScroll& e);
		bool onMouseClick(event::MouseButtonPress& e);
		bool onMouseRelease(event::MouseButtonRelease& e);

		void setSpeed(float speed) { m_speed = speed; }
		float getSpeed() const { return m_speed; }
		void setSensetivity(float sensetivity) { m_sensetivity = sensetivity; }
		float getSensetivity() { return m_sensetivity; }

		void setCamera(Traits<Camera>::OPtr camera_to_control) { m_camera = camera_to_control; }

	private:

		Traits<Camera>::OPtr m_camera;
		float m_speed;
		float m_sensetivity;

		std::pair<float, float> m_last_mouse_pos;

		bool m_first_mouse_event = true;
		bool m_camera_active = false;

	};
}