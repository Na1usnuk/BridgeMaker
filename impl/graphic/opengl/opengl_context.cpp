module;

#include "glad/glad.h"
#include "GLFW/glfw3.h"

module bm.gfx.context;

import bm.window;
import bm.log;
import bm.assert;

namespace bm::gfx
{

void Context::init()
{
	auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	core::verify(status, "Failed to initialize Glad");
	log::core::info("Glad initialized");
}

void Context::makeCurrent(Window* window)
{
	if (m_window == window)
		return;
	m_window = window;
	glfwMakeContextCurrent(m_window->getNativeWindow());
}

Window* Context::getCurrent()
{
	core::verify(m_window, "Current context is invalid");
	return m_window;
}

void Context::destroy()
{
	glfwTerminate();
}

void Context::swapBuffers() const { glfwSwapBuffers(m_window->getNativeWindow()); }

Context::NativeWindow Context::shareContext() const
{
	if (m_window != nullptr)
		return m_window->getNativeWindow();
	return nullptr;
}

Context& Context::getContext()
{
	static Context ctx;
	return ctx;
}

}
