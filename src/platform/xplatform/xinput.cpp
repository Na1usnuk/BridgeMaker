#include "pch.hpp"

#include "platform/xplatform/xinput.hpp"
#include "platform/window.hpp"
#include "platform/xplatform/xwindow.hpp"

#include "GLFW/glfw3.h"

BM_START

bool XInput::isPressed(Window& window, int key)
{
	return (glfwGetKey(window.getNativeWindow(), key) == GLFW_PRESS);
}

bool XInput::isReleased(Window& window, int key)
{
	return false;
}


BM_END

