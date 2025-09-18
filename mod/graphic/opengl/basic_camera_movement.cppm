export module bm.camerainp;

import bm.gfx.camera;
import bm.event;
import bm.input;
import bm.log;
import bm.app;

import bm.layer.base;

import std;

namespace bm
{

	export class DefaultCameraInputLayer : public Layer
	{
	public:

		DefaultCameraInputLayer(gfx::CameraPtr camera_to_control = nullptr, float speed = 3.0f, float sensetivity = .5f)
			: m_camera(camera_to_control), 
			  m_speed(speed), 
			  m_sensetivity(sensetivity),
			  m_last_mouse_pos(Input::getMousePos())
		{
		}

		void onUpdate(float delta_time) override;
		void onEvent(Event& e) override;

		bool onMouseMove(MouseMoveEvent& e);
		bool onScroll(MouseScrollEvent& e);
		bool onMouseClick(MouseButtonPressEvent& e);
		bool onMouseRelease(MouseButtonReleaseEvent& e);

		void setSpeed(float speed) { m_speed = speed; }
		float getSpeed() const { return m_speed; }
		void setSensetivity(float sensetivity) { m_sensetivity = sensetivity; }
		float getSensetivity() { return m_sensetivity; }

		void setCamera(gfx::CameraPtr camera_to_control) { m_camera = camera_to_control; }

	private:

		std::weak_ptr<gfx::Camera> m_camera;
		float m_speed; 
		float m_sensetivity;

		std::pair<float, float> m_last_mouse_pos;

		bool m_first_mouse_event = true;
		bool m_need_view_recalculation = false;
		bool m_camera_active = false;

	};

	void DefaultCameraInputLayer::onEvent(Event& e)
	{

		EventDispatcher d(e);

		d.dispatch<MouseMoveEvent>(bindEventFn(&DefaultCameraInputLayer::onMouseMove, this));
		d.dispatch<MouseScrollEvent>(bindEventFn(&DefaultCameraInputLayer::onScroll, this));
		d.dispatch<MouseButtonPressEvent>(bindEventFn(&DefaultCameraInputLayer::onMouseClick, this));
		d.dispatch<MouseButtonReleaseEvent>(bindEventFn(&DefaultCameraInputLayer::onMouseRelease, this));
	}

	bool DefaultCameraInputLayer::onMouseClick(MouseButtonPressEvent& e)
	{
		if (Input::Mouse::RIGHT != e.getKey())
			return false;

		Application::get().getWindow().setCaptureCursor(true);
		m_camera_active = true;
		return true;
	}

	bool DefaultCameraInputLayer::onMouseRelease(MouseButtonReleaseEvent& e)
	{
		if (Input::Mouse::RIGHT != e.getKey())
			return false;

		Application::get().getWindow().setCaptureCursor(false);
		m_camera_active = false;
		m_first_mouse_event = true;
		return true;
	}

	bool DefaultCameraInputLayer::onScroll(MouseScrollEvent& e)
	{
		auto camera = m_camera.lock();
		if (not camera)
			return false;

		if (!m_camera_active)
			return false;

		camera->setFOV(std::clamp(camera->getFOV() - e.getY(), 1.f, 65.f));
		camera->recalculateProjection();

		return false;
	}

	bool DefaultCameraInputLayer::onMouseMove(MouseMoveEvent& e)
	{
		auto camera = m_camera.lock();
		if (not camera)
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

		camera->setYaw(camera->getYaw() + delta_x);
		camera->setPitch(std::clamp(camera->getPitch() + delta_y, -89.0f, 89.0f));

		m_need_view_recalculation = true;

		return true;
	}

	void DefaultCameraInputLayer::onUpdate(float delta_time)
	{
		if (!m_camera_active)
			return;

		auto camera = m_camera.lock();
		if (not camera)
			return;

		float speed = m_speed * delta_time;

		if (Input::isPressed(Input::Key::W))
		{
			camera->setPosition(camera->getPosition() + (speed * camera->getDirection()) );
			m_need_view_recalculation = true;
		}
		if (Input::isPressed(Input::Key::S))
		{
			camera->setPosition(camera->getPosition() - (speed * camera->getDirection()));
			m_need_view_recalculation = true;
		}
		if (Input::isPressed(Input::Key::A))
		{
			camera->setPosition(camera->getPosition() + (speed * glm::normalize(camera->getRight() * glm::vec3(1.f, 0.f, 1.f))));
			m_need_view_recalculation = true;
		}
		if (Input::isPressed(Input::Key::D))
		{
			camera->setPosition(camera->getPosition() - (speed * glm::normalize(camera->getRight() * glm::vec3(1.f, 0.f, 1.f))));
			m_need_view_recalculation = true;
		}
		if (Input::isPressed(Input::Key::E) or Input::isPressed(Input::Key::SPACE))
		{
			camera->setPosition(camera->getPosition() + (speed * camera->getUp()));
			
			m_need_view_recalculation = true;
		}
		if (Input::isPressed(Input::Key::Q) or Input::isPressed(Input::Key::LEFT_CONTROL))
		{
			camera->setPosition(camera->getPosition() - (speed * camera->getUp()));
			
			m_need_view_recalculation = true;
		}

		if (m_need_view_recalculation)
			camera->recalculateView();
		
	}

}