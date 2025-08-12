#include "pch.hpp"
#include "application.hpp"
#include "log.hpp"

#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"


BM_START

Application::Application()
{
}

Application::~Application()
{
}

void Application::onEvent(Event& e)
{
}

void Application::onLayersEvent(Event& e)
{
	for (auto layer = m_layers.end(); layer > m_layers.begin();)
	{
		(*--layer)->onEvent(e);
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

			onEvent(e);
			for (auto& l : m_layers)
			{
				l->onEvent(e);
				l->onUpdate();
			}
			onUpdate();
			w.onUpdate();
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


Window& Application::addWindow(std::string_view title, int width, int height, bool vsync)
{
	m_windows.emplace_back(title, width, height, vsync);
	m_windows.back().setEventCallback(BM_BIND_EVENT_FN(Application::onEvent));
	return m_windows.back();
}

void Application::_closeWindow(Window* window)
{
	auto it = std::find(m_windows.begin(), m_windows.end(), *window);
	if (it != m_windows.end())
		m_windows.erase(it);
}

BM_END