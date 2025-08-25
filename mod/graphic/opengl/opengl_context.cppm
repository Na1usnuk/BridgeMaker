module;

#include "GLFW/glfw3.h"

export module bm.gfx.context;

import bm.window;

namespace bm::gfx
{

export class Context
{

public:

	using NativeWindow = GLFWwindow*;

public:

	void init();
	void destroy();
	void makeCurrent(Window* window);
	Window* getCurrent();
	void swapBuffers() const;
	NativeWindow shareContext() const;

	static Context& getContext();

private:

	Context()
	{
	}

	Window* m_window = nullptr;

};


}



