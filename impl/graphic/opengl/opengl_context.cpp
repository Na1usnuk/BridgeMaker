module;

#include "glad/glad.h"
#include "GLFW/glfw3.h"

module bm.gfx.context;

import bm.window;
import bm.log;
import bm.assert;

namespace bm::gfx
{

Window* Context::s_window = nullptr;

void Context::init()
{
	auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	core::verify(status, "Failed to initialize Glad");
	log::core::info("Glad initialized");
}

void Context::makeCurrent(Window* window)
{
	if (s_window == window)
		return;
	s_window = window;
	glfwMakeContextCurrent(s_window->getNativeWindow());
}

Window* Context::getCurrent()
{
	core::verify(s_window, "Current context is invalid");
	return s_window;
}

void Context::destroy()
{
	glfwTerminate();
}

void Context::swapBuffers() { glfwSwapBuffers(s_window->getNativeWindow()); }

Context::NativeWindow Context::shareContext()
{
	if (s_window != nullptr)
		return s_window->getNativeWindow();
	return nullptr;
}

}
