module bm.app:controller;
import :controller;
import :frame;

import bm.platform;
import bm.gfx;
import bm.core;

namespace bm::app
{

	DebugCameraController::DebugCameraController(gfx::Camera& camera_to_control, const platform::Window& window, float speed, float sensitivity) :
		m_camera(&camera_to_control),
		m_input(window),
		m_speed(speed),
		m_sensitivity(sensitivity),
		m_last_mouse_pos(m_input.getMousePosition())
	{}

	void DebugCameraController::onEvent(Event& e)
	{
		EventDispatcher d(e);

		d.dispatch<MouseMoveEvent>(bindEventFunc(&DebugCameraController::onMouseMove, this));
		d.dispatch<MouseScrollEvent>(bindEventFunc(&DebugCameraController::onScroll, this));
		d.dispatch<MouseButtonPressEvent>(bindEventFunc(&DebugCameraController::onMouseClick, this));
		d.dispatch<MouseButtonReleaseEvent>(bindEventFunc(&DebugCameraController::onMouseRelease, this));
	}

	bool DebugCameraController::onMouseClick(MouseButtonPressEvent& e)
	{
		if (platform::Input::Mouse::Right != e.getButton())
			return false;

		m_camera_active = true;
		return true;
	}

	bool DebugCameraController::onMouseRelease(MouseButtonReleaseEvent& e)
	{
		if (platform::Input::Mouse::Right != e.getButton())
			return false;

		m_camera_active = false;
		m_first_mouse_event = true;
		return true;
	}

	bool DebugCameraController::onScroll(MouseScrollEvent& e)
	{
		if (not m_camera_active)
			return false;

		m_camera->setFOV(std::clamp(m_camera->getFOV() - e.getY(), 1., 65.));

		return false;
	}

	bool DebugCameraController::onMouseMove(MouseMoveEvent& e)
	{
		if (not m_camera_active)
		{
			m_last_mouse_pos = { e.getX(), e.getY() };
			return false;
		}

		if (m_first_mouse_event)
		{
			m_last_mouse_pos = { e.getX(), e.getY() };
			m_first_mouse_event = false;
		}

		float delta_x = (e.getX() - m_last_mouse_pos.first) * m_sensitivity;
		float delta_y = (m_last_mouse_pos.second - e.getY()) * m_sensitivity;
		m_last_mouse_pos = { e.getX(), e.getY() };

		auto yaw = m_camera->getYaw() + delta_x;
		if (yaw > 360) yaw -= 360;
		if (yaw < 0)   yaw += 360;
		m_camera->setYaw(yaw);
		m_camera->setPitch(std::clamp(m_camera->getPitch() + delta_y, -89.0f, 89.0f));

		return true;
	}

	void DebugCameraController::onUpdate(DeltaTime delta_time)
	{
		if (not m_camera_active)
			return;

		float speed = m_speed * delta_time.count();

		glm::vec3 front = m_camera->getDirection();
		glm::vec3 right = m_camera->getRight(front);
		glm::vec3 world_up = m_camera->getWorldUp();

		if (m_input.isPressed(platform::Input::Key::LeftShift))
			speed *= 2.0f;

		if (m_input.isPressed(platform::Input::Key::W))
			m_camera->setPosition(m_camera->getPosition() + front * speed);

		if (m_input.isPressed(platform::Input::Key::S))
			m_camera->setPosition(m_camera->getPosition() - front * speed);

		if (m_input.isPressed(platform::Input::Key::A))
			m_camera->setPosition(m_camera->getPosition() - right * speed);

		if (m_input.isPressed(platform::Input::Key::D))
			m_camera->setPosition(m_camera->getPosition() + right * speed);

		if (m_input.isPressed(platform::Input::Key::E) || m_input.isPressed(platform::Input::Key::Space))
			m_camera->setPosition(m_camera->getPosition() + world_up * speed);

		if (m_input.isPressed(platform::Input::Key::Q) || m_input.isPressed(platform::Input::Key::LeftControl))
			m_camera->setPosition(m_camera->getPosition() - world_up * speed);

	}
}