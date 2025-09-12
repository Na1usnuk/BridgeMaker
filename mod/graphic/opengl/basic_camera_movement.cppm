export module bm.camerainp;

import bm.gfx.camera;
import bm.event;
import bm.input;
import bm.log;

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

		BasicCameraMovement(gfx::Camera* camera_to_control = nullptr, float speed = 1.0f) : m_camera(camera_to_control), m_speed(speed)
		{
		}

		void onUpdate(float delta_time);

		void onEvent(Event& e);
		bool onKeyPress(KeyPressedEvent& e);
		bool onKeyRelease(KeyReleasedEvent& e);

		void setSpeed(float speed) { m_speed = speed; }
		float getSpeed() const { return m_speed; }

		void setCamera(gfx::Camera* camera_to_control) { m_camera = camera_to_control; }

	private:

		gfx::Camera* m_camera;
		float m_speed; 

		MoveState m_move_state = MoveState::None;

	};

	void BasicCameraMovement::onUpdate(float delta_time)
	{
		float speed = m_speed * delta_time;
		switch (m_move_state)
		{
		case MoveState::None:
		{
			return;
			break;
		}
		case MoveState::Left:
		{
			m_camera->moveLeft(speed);
			break;
		}
		case MoveState::Right:
		{
			m_camera->moveRight(speed);
			break;
		}
		case MoveState::Forward:
		{
			m_camera->moveUp(speed);
			break;
		}
		case MoveState::Back:
		{
			m_camera->moveDown(speed);
			break;
		}
		default:
		{
			return;
			break;
		}
		}
		m_camera->recalculateView();
	}

	void BasicCameraMovement::onEvent(Event& e)
	{
		if (m_camera == nullptr)
			return;

		EventDispatcher d(e);

		d.dispatch<KeyPressedEvent>(bindEventFn(&BasicCameraMovement::onKeyPress, this));
		d.dispatch<KeyReleasedEvent>(bindEventFn(&BasicCameraMovement::onKeyRelease, this));
	}

	bool BasicCameraMovement::onKeyPress(KeyPressedEvent& e)
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

	bool BasicCameraMovement::onKeyRelease(KeyReleasedEvent& e)
	{
		m_move_state = MoveState::None;
		return true;
	}

}