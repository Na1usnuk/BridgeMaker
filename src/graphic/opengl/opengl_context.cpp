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
	BM_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize Glad");
	BM_CORE_INFO("Glad initialized");
}

void OpenGLContext::makeCurrent(XWindow* window)
{
	m_window = window;
	glfwMakeContextCurrent(m_window->getNativeWindow());
}

void OpenGLContext::swapBuffers() const { glfwSwapBuffers(m_window->getNativeWindow()); }

GLFWwindow* OpenGLContext::shareContext() const
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