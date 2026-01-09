module;

#include "GLFW/glfw3.h"

module bm.platform:input;
import :input;
import :window;

import std;

namespace bm 
{

	Input::Input(const Window& window) noexcept : 
		m_window(window)
	{ }

	bool Input::isPressed(Key key) const noexcept
	{
		return glfwGetKey(m_window.getNative(), std::to_underlying(key)) == GLFW_PRESS;
	}
	
	bool Input::isReleased(Key key) const noexcept
	{
		return glfwGetKey(m_window.getNative(), std::to_underlying(key)) == GLFW_RELEASE;
	}
	
	bool Input::isPressed(Mouse button) const noexcept
	{
		return glfwGetMouseButton(m_window.getNative(), std::to_underlying(button)) == GLFW_PRESS;
	}
	
	bool Input::isReleased(Mouse button) const noexcept
	{
		return glfwGetMouseButton(m_window.getNative(), std::to_underlying(button)) == GLFW_RELEASE;
	}

	std::pair<double, double> Input::getMousePosition() const noexcept
	{
		double x, y;
		glfwGetCursorPos(m_window.getNative(), &x, &y);
		return { x, y };
	}

}

