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

	static void init();
	static void destroy();
	static void makeCurrent(Window* window);
	static Window* getCurrent();
	static void swapBuffers();
	static NativeWindow shareContext();

private:

	static Window* s_window;

};


}



