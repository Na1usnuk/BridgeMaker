#include "pch.hpp"
#include "application.hpp"
#include "log.hpp"

#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"


BM_START

Application::Application(std::string_view title, int width, int height)
	:	m_window(Window::Data(title, width, height))
{
	m_window.setEventCallback(BM_BIND_EVENT_FN(Application::onEvent));
}

Application::~Application()
{
	//m_window.destroy();
}

void Application::onEvent(Event& e)
{
	EventDispatcher d(e);

	d.dispatch<WindowCloseEvent>(BM_BIND_EVENT_FN(Application::onClose));

	onLayersEvent(e);
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

bool Application::onClose(WindowCloseEvent& e)
{
	m_window.close();
	return true;
}

bool Application::onResize(WindowResizeEvent& e)
{
	m_window.resize(e.getWidth(), e.getHeight());
	return true;
}

void Application::onUpdate()
{
	for (auto& window : m_windows)
		window.onUpdate();
	for (auto& layer : m_layers)
		layer->onUpdate();
}

int Application::run(int argc, char** argv)
{

	while (m_window.isOpen())
	{
		onUpdate();
		m_window.onUpdate();
	}

	return EXIT_SUCCESS;
}

void Application::closeWindow(const Window* window)
{
	auto it = std::find(m_windows.begin(), m_windows.end(), *window);
	if (it != m_windows.end())
		m_windows.erase(it);
}

void Application::addWindow(std::string_view title, int width, int height)
{
	m_windows.emplace_back(title, width, height);
	m_windows.back().setEventCallback(BM_BIND_EVENT_FN(Application::onEvent));
}

BM_END