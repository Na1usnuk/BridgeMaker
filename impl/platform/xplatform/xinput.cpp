module;

#include "GLFW/glfw3.h"

module bm.platform:input;
import :input;
import :window;

namespace bm {

	Window* Input::s_window = nullptr;
	
	std::pair<float, float> Input::s_mouse_pos = { .0f, .0f };
	
	bool Input::isPressed(Key key)
	{
		return isPressedImpl(*s_window, static_cast<int>(key));
	}
	
	bool Input::isReleased(Key key)
	{
		return isReleasedImpl(*s_window, static_cast<int>(key));
	}
	
	bool Input::isPressed(Mouse button)
	{
		return isPressedImpl(*s_window, static_cast<int>(button));
	}
	
	bool Input::isReleased(Mouse button)
	{
		return isReleasedImpl(*s_window, static_cast<int>(button));
	}
	
	std::pair<float, float> Input::getMousePosition()
	{
		return s_mouse_pos;
	}
	
	void Input::setMousePosition(const std::pair<float, float>& pos)
	{
		s_mouse_pos = pos;
	}
	
	bool Input::isPressedImpl(Window& window, int key)
	{
		if (glfwGetKey(window.getNative(), key) == GLFW_PRESS)
			return true;
		return false;
	}
	
	bool Input::isReleasedImpl(Window& window, int key)
	{
		if (glfwGetKey(window.getNative(), key) == GLFW_RELEASE)
			return true;
		return false;
	}

	namespace event
	{
		std::string MouseMove::toString() const
		{
			std::stringstream ss;
			ss << "MouseMove: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		std::string MouseScroll::toString() const
		{
			std::stringstream ss;
			ss << "MouseScroll: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}

		std::string MouseButtonPress::toString() const
		{
			std::stringstream ss;
			ss << "MouseButtonPress: " << static_cast<int>(m_button);
			return ss.str();
		}

		std::string MouseButtonRelease::toString() const
		{
			std::stringstream ss;
			ss << "MouseButtonRelease: " << static_cast<int>(m_button);
			return ss.str();
		}

		std::string KeyPress::toString() const
		{
			std::stringstream ss;
			ss << "KeyPress: " << static_cast<int>(m_key) << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		std::string KeyRelease::toString() const
		{
			std::stringstream ss;
			ss << "KeyRelease: " << static_cast<int>(m_key);
			return ss.str();
		}
	}

}

