#pragma once

#include "core/core.hpp"


BM_START

template<typename Backend>
class AbstractContext 
{
public:


	void init() { Backend::getContext().init(); }
	template<typename WindowType>
	void makeCurrent(WindowType* window) { Backend::getContext().makeCurrent(&window->getImpl()); }
	Backend::native_window_t shareContext() { return Backend::getContext().shareContext(); }
	void swapBuffers() const { Backend::getContext().swapBuffers(); }

	static AbstractContext& getContext() { static AbstractContext ctx; return ctx; }

private:

	AbstractContext() = default;

};

BM_END

