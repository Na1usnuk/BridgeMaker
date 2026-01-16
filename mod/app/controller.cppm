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

		DebugCameraController(gfx::Camera& camera_to_control, const platform::Input& input, float speed = 3.0f, float sensetivity = .5f);

		//void onUpdate(DeltaTime delta_time);
		//void onEvent(Event& e);

		//bool onMouseMove(MouseMoveEvent& e);
		//bool onScroll(MouseScrollEvent& e);
		//bool onMouseClick(MouseButtonPressEvent& e);
		//bool onMouseRelease(MouseButtonReleaseEvent& e);
		//
		//void setSpeed(float speed) { m_speed = speed; }
		//float getSpeed() const { return m_speed; }
		//void setSensetivity(float sensetivity) { m_sensetivity = sensetivity; }
		//float getSensetivity() { return m_sensetivity; }

		//void setCamera(Traits<gfx::Camera>::OPtr camera_to_control) { m_camera = camera_to_control; }

	private:

		gfx::Camera* m_camera;
		const platform::Input& m_input;

		float m_speed;
		float m_sensetivity;

		std::pair<float, float> m_last_mouse_pos;

		bool m_first_mouse_event = true;
		bool m_camera_active = false;

	};
}