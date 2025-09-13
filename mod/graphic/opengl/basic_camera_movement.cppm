export module bm.camerainp;

import bm.gfx.camera;
import bm.event;
import bm.input;
import bm.log;

import std;

namespace bm
{

	export class BasicCameraMovement
	{
	public:

		enum class MoveState
		{
			None = 0,
			Forward,
			Back,
			Left, 
			Right,
			Up,
			Down,
		};

	public:

		BasicCameraMovement(gfx::Camera* camera_to_control = nullptr, float speed = 1.0f, float sensetivity = .5f) 
			: m_camera(camera_to_control), 
			  m_speed(speed), 
			  m_sensetivity(sensetivity),
			  m_last_mouse_pos(Input::getMousePos())
		{
		}

		void onUpdate(float delta_time);

		void onEvent(Event& e);
		bool onKeyPress(KeyPressEvent& e);
		bool onKeyRelease(KeyReleaseEvent& e);
		bool onMouseMove(MouseMoveEvent& e);
		bool onScroll(MouseScrollEvent& e);

		void setSpeed(float speed) { m_speed = speed; }
		float getSpeed() const { return m_speed; }

		void setCamera(gfx::Camera* camera_to_control) { m_camera = camera_to_control; }

	private:

		gfx::Camera* m_camera;
		float m_speed; 
		float m_sensetivity;

		std::pair<float, float> m_last_mouse_pos;

		bool m_first_mouse_event = true;
		bool m_need_view_recalculation = false;

		MoveState m_move_state = MoveState::None;

	};

	void BasicCameraMovement::onEvent(Event& e)
	{
		if (m_camera == nullptr)
			return;

		EventDispatcher d(e);

		d.dispatch<KeyPressEvent>(bindEventFn(&BasicCameraMovement::onKeyPress, this));
		d.dispatch<KeyReleaseEvent>(bindEventFn(&BasicCameraMovement::onKeyRelease, this));

		d.dispatch<MouseMoveEvent>(bindEventFn(&BasicCameraMovement::onMouseMove, this));

		d.dispatch<MouseScrollEvent>(bindEventFn(&BasicCameraMovement::onScroll, this));
	}

	bool BasicCameraMovement::onScroll(MouseScrollEvent& e)
	{
		m_camera->setFOV(std::clamp(m_camera->getFOV() - e.getY(), 1.f, 65.f));
		m_camera->recalculateProjection();

		return false;
	}

	bool BasicCameraMovement::onMouseMove(MouseMoveEvent& e)
	{
		if (m_first_mouse_event)
		{
			m_last_mouse_pos = { e.getX(), e.getY() };
			m_first_mouse_event = false;
		}

		float delta_x = (e.getX() - m_last_mouse_pos.first) * m_sensetivity;
		float delta_y = (m_last_mouse_pos.second - e.getY()) * m_sensetivity;
		m_last_mouse_pos = { e.getX(), e.getY() };

		m_camera->setYaw(m_camera->getYaw() + delta_x);
		m_camera->setPitch(std::clamp(m_camera->getPitch() + delta_y, -89.0f, 89.0f));

		m_need_view_recalculation = true;

		return true;
	}

	void BasicCameraMovement::onUpdate(float delta_time)
	{
		float speed = m_speed * delta_time;
		switch (m_move_state)
		{
		case MoveState::Left:
		{
			m_camera->moveLeft(speed);
			m_need_view_recalculation = true;
			break;
		}
		case MoveState::Right:
		{
			m_camera->moveRight(speed);
			m_need_view_recalculation = true;
			break;
		}
		case MoveState::Forward:
		{
			m_camera->moveForward(speed);
			m_need_view_recalculation = true;
			break;
		}
		case MoveState::Back:
		{
			m_camera->moveBack(speed);
			m_need_view_recalculation = true;
			break;
		}
		case MoveState::Up:
		{
			m_camera->moveUp(speed);
			m_need_view_recalculation = true;
			break;
		}
		case MoveState::Down:
		{
			m_camera->moveDown(speed);
			m_need_view_recalculation = true;
			break;
		}
		}
		if(m_need_view_recalculation)
		{
			m_camera->recalculateView();
			m_need_view_recalculation = false;
		}
	}


	bool BasicCameraMovement::onKeyPress(KeyPressEvent& e)
	{
		switch (e.getKey())
		{
		case Input::Key::W:
		{
			m_move_state = MoveState::Forward;
			break;
		}
		case Input::Key::S:
		{
			m_move_state = MoveState::Back;
			break;
		}
		case Input::Key::A:
		{
			m_move_state = MoveState::Left;
			break;
		}
		case Input::Key::D:
		{
			m_move_state = MoveState::Right;
			break;
		}
		case Input::Key::E:
		{
			m_move_state = MoveState::Up;
			break;
		}
		case Input::Key::Q:
		{
			m_move_state = MoveState::Down;
			break;
		}
		}

		return true;
	}

	bool BasicCameraMovement::onKeyRelease(KeyReleaseEvent& e)
	{
		m_move_state = MoveState::None;
		return true;
	}

}