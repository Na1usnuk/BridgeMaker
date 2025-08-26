module bm.application;

import bm.event.base;
import bm.event.app;
import bm.window;
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
			gfx::Context::makeCurrent(&w);

			onUpdate();
			for (auto& l : m_layers)
				if(l->isEnabled())
					l->onUpdate();

			onEvent(e);

			gfx::Context::swapBuffers();
			w.onUpdate(); // poll events
		}
		if (m_close_window != nullptr)
		{
			_closeWindow(m_close_window);
			m_close_window = nullptr;
			continue;
		}
	}
	return 0;
}

void Application::closeWindow(Window* window)
{
		m_close_window = window;
}

Window& Application::addWindow(std::string_view title, int width, int height, bool vsync, bool decorated, bool visible)
{
	m_windows.emplace_back(title, width, height, vsync, decorated, visible);
	m_windows.back().setEventCallback(bindEventFunc(&Application::onEvent, this));
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