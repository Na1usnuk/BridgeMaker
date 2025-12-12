module bm.platform:controller;
import :controller;

import :input;

import bm.core;

namespace bm
{

	DebugCameraController::DebugCameraController(Traits<Camera>::OPtr camera_to_control, float speed, float sensetivity)
		: Layer("Debug Camera Controller"),
		m_camera(camera_to_control),
		m_speed(speed),
		m_sensetivity(sensetivity),
		m_last_mouse_pos(Input::getMousePosition())
	{}

	void DebugCameraController::onEvent(event::Event& e)
	{
		using namespace bm::event;
		Dispatcher d(e);

		d.dispatch<MouseMove>(bindEventFunc(&DebugCameraController::onMouseMove, this));
		d.dispatch<MouseScroll>(bindEventFunc(&DebugCameraController::onScroll, this));
		d.dispatch<MouseButtonPress>(bindEventFunc(&DebugCameraController::onMouseClick, this));
		d.dispatch<MouseButtonRelease>(bindEventFunc(&DebugCameraController::onMouseRelease, this));
	}

	bool DebugCameraController::onMouseClick(event::MouseButtonPress& e)
	{
		if (Input::Mouse::RIGHT != e.getKey())
			return false;

		//Application::get().getWindow().setCaptureCursor(true);
		m_camera_active = true;
		return true;
	}

	bool DebugCameraController::onMouseRelease(event::MouseButtonRelease& e)
	{
		if (Input::Mouse::RIGHT != e.getKey())
			return false;

		//Application::get().getWindow().setCaptureCursor(false);
		m_camera_active = false;
		m_first_mouse_event = true;
		return true;
	}

	bool DebugCameraController::onScroll(event::MouseScroll& e)
	{
		if (not m_camera)
			return false;

		if (!m_camera_active)
			return false;

		m_camera->setFOV(std::clamp(m_camera->getFOV() - e.getY(), 1.f, 65.f));

		return false;
	}

	bool DebugCameraController::onMouseMove(event::MouseMove& e)
	{
		if (not m_camera)
			return false;

		if (!m_camera_active)
		{
			m_last_mouse_pos = { e.getX(), e.getY() };
			return false;
		}

		if (m_first_mouse_event)
		{
			m_last_mouse_pos = { e.getX(), e.getY() };
			m_first_mouse_event = false;
		}

		float delta_x = (e.getX() - m_last_mouse_pos.first) * m_sensetivity;
		float delta_y = (m_last_mouse_pos.second - e.getY()) * m_sensetivity;
		m_last_mouse_pos = { e.getX(), e.getY() };

		auto yaw = m_camera->getYaw() + delta_x;
		if (yaw > 360) yaw -= 360;
		if (yaw < 0)   yaw += 360;
		m_camera->setYaw(yaw);
		m_camera->setPitch(std::clamp(m_camera->getPitch() + delta_y, -89.0f, 89.0f));

		return true;
	}

	void DebugCameraController::onUpdate(float delta_time)
	{
		if (!m_camera_active)
			return;

		if (not m_camera)
			return;

		float speed = m_speed * delta_time;

		glm::vec3 front = m_camera->getDirection();
		glm::vec3 right = m_camera->getRight(front);
		glm::vec3 world_up = m_camera->getWorldUp();

		if (Input::isPressed(Input::Key::LEFT_SHIFT))
			speed *= 2.0f;

		if (Input::isPressed(Input::Key::W))
			m_camera->setPosition(m_camera->getPosition() + front * speed);

		if (Input::isPressed(Input::Key::S))
			m_camera->setPosition(m_camera->getPosition() - front * speed);

		if (Input::isPressed(Input::Key::A))
			m_camera->setPosition(m_camera->getPosition() - right * speed);

		if (Input::isPressed(Input::Key::D))
			m_camera->setPosition(m_camera->getPosition() + right * speed);

		if (Input::isPressed(Input::Key::E) || Input::isPressed(Input::Key::SPACE))
			m_camera->setPosition(m_camera->getPosition() + world_up * speed);

		if (Input::isPressed(Input::Key::Q) || Input::isPressed(Input::Key::LEFT_CONTROL))
			m_camera->setPosition(m_camera->getPosition() - world_up * speed);

	}
}