#pragma once

#include "core/core.hpp"

struct GLFWwindow;

BM_START
GL_START



class OpenGLContext
{
public:

	using native_window_t = GLFWwindow*;

public:

	void init();
	void destroy();
	void makeCurrent(XWindow* window);
	XWindow* getCurrent();
	void swapBuffers() const;
	GLFWwindow* shareContext() const;

	static OpenGLContext& getContext();

private:

	OpenGLContext()
	{
	}

	XWindow* m_window = nullptr;

};


GL_END
BM_END


