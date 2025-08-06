#include "pch.hpp"
#include "application.hpp"
#include "log.hpp"

#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"


BM_START

Application::Application(std::string_view title, int width, int height)
	:	m_window(WindowsWindow::Data(title, width, height))
{
	m_window.setEventCallback(BM_BIND_EVENT_FN(Application::onEvent));
}


Application::~Application()
{
}


void Application::onEvent(Event& e)
{
	EventDispatcher d(e);

	d.dispatch<WindowCloseEvent>(BM_BIND_EVENT_FN(Application::onClose));

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

BM_END