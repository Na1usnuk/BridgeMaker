module bm.app;

import bm.core;

namespace bm::app
{
	Application::Application(std::string_view title, int width, int height, bool decorated, bool visible) :
		window(title, width, height, decorated, visible),
		m_frame_timer(420)
	{
		event_system.setEventCallback([this](Event& e) { onEvent(e); });
	}

	int Application::run(int argc, char** argv)
	{
		onArguments(argc, argv);
		return run();
	}

	int Application::run()
	{
		onStartup();

		while (m_is_running)
		{
			m_frame_timer.startFrame();

			event_system.pollEvents();
			onUpdate(m_frame_timer.getDeltaTime());
			onRender();

			graphic_context.swapBuffers();

			m_frame_timer.endFrame();
		}

		onShutdown();

		return return_code;
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher d(e);
		d.dispatch<WindowCloseEvent>([this](WindowCloseEvent& c) {close(); return true; });
	}
}