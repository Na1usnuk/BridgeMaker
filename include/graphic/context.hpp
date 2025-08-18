#pragma once

#include "core/core.hpp"
#include "core/log.hpp"

BM_START

template<typename Backend>
class AbstractContext 
{
public:


	void init() { Backend::getContext().init(); }
	template<typename WindowType = Window>
	void makeCurrent(WindowType* window) { Backend::getContext().makeCurrent(&window->getImpl()); }
	template<typename WindowType = Window>
	WindowType* getCurrent() { return Backend::getContext().getCurrent()->getSelf(); }
	typename Backend::NativeWindow shareContext() { return Backend::getContext().shareContext(); }
	void swapBuffers() const { Backend::getContext().swapBuffers(); }

	static AbstractContext& getContext() { static AbstractContext ctx; return ctx; }

private:


};

BM_END

