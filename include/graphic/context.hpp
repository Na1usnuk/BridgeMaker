#pragma once

#include "core/core.hpp"

class Window;

BM_START

template<typename Backend>
class Context 
{
public:


	void init() { Backend::getContext().init(); }
	void makeCurrent(Window* window) { Backend::getContext().makeCurrent(window); }
	Backend::native_window_t shareContext() { return Backend::getContext().ishareContext(); }
	void swapBuffers() const { Backend::getContext().iswapBuffers(); }

	static Context& getContext() { static Context ctx; return ctx; }

private:

	Context() = default;

};

BM_END

