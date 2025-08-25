#include "pch.hpp"
#include "application.hpp"

#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"

import bm.log;

namespace bm 
{

Application::~Application()
{
}

void Application::onEvent(Event& e)
{
	//EventDispatcher d(e);
	onLayersEvent(e);
}

void Application::onLayersEvent(Event& e)
{
	for (auto layer = m_layers.end(); layer > m_layers.begin();)
	{
		if (!(*--layer)->isEnabled()) continue;
		(*layer)->onEvent(e);
		if (e.isHandled())
			break;
	}
}

int Application::run(int argc, char** argv)
{
	//processArgs(argc, argv);

	while (m_is_running)
	{
		for (auto& w : m_windows)
		{
			AppRenderEvent e;
			e.setWindow(&w);
			m_ctx.makeCurrent(&w);

			onUpdate();
			for (auto& l : m_layers)
				if(l->isEnabled())
					l->onUpdate();

			onEvent(e);

			m_ctx.swapBuffers();
			w.onUpdate(); // poll events
		}
		if (m_close_window != nullptr)
		{
			_closeWindow(m_close_window);
			m_close_window = nullptr;
			continue;
		}
	}
	return EXIT_SUCCESS;
}

void Application::closeWindow(Window* window)
{
		m_close_window = window;
}

Window& Application::addWindow(std::string_view title, int width, int height, bool vsync, bool decorated, bool visible)
{
	m_windows.emplace_back(title, width, height, vsync, decorated, visible);
	m_windows.back().setEventCallback(BM_BIND_EVENT_FN(Application::onEvent));
	return m_windows.back();
}

Application::Application()
{

}

void Application::_closeWindow(Window* window)
{
	auto it = std::find(m_windows.begin(), m_windows.end(), *window);
	if (it != m_windows.end())
		m_windows.erase(it);
}

}