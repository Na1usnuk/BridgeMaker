module;

#include "GLFW/glfw3.h"

module bm.input;

import bm.window;

namespace bm {

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

std::pair<float, float> Input::getMousePos()
{
	return std::pair<float, float>();
}

void Input::setMousePos(const std::pair<float, float>& pos)
{
}

bool Input::isPressedImpl(Window& window, int key)
{
	if (glfwGetKey(window.getNativeWindow(), key) == GLFW_PRESS)
		return true;
	return false;
}

bool Input::isReleasedImpl(Window& window, int key)
{
	if (glfwGetKey(window.getNativeWindow(), key) == GLFW_RELEASE)
		return true;
	return false;
}

}

