#include "pch.hpp"

#include "log.hpp"
#include "opengl/opengl_context.hpp"
#include "platform/window.hpp"
#include "platform/xplatform/xwindow.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


BM_START
GL_START

void OpenGLContext::init()
{
	auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	BM_CORE_ASSERT(status, "Failed to initialize Glad");
	BM_CORE_INFO("Glad initialized");
}

void OpenGLContext::makeCurrent(XWindow* window)
{
	if (m_window == window)
		return;
	m_window = window;
	glfwMakeContextCurrent(m_window->getNativeWindow());
}

XWindow* OpenGLContext::getCurrent()
{
	BM_CORE_ASSERT(m_window, "Current context is invalid");
	return m_window;
}

void OpenGLContext::destroy()
{
	glfwTerminate();
}

void OpenGLContext::swapBuffers() const { glfwSwapBuffers(m_window->getNativeWindow()); }

OpenGLContext::NativeWindow OpenGLContext::shareContext() const
{
	if (m_window != nullptr)
		return m_window->getNativeWindow();
	return nullptr;
}

OpenGLContext& OpenGLContext::getContext()
{
	static OpenGLContext ctx;
	return ctx;
}

GL_END
BM_END