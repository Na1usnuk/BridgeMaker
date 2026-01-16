module bm.app;

import bm.core;

namespace bm
{
	Application::Application(std::string_view title, int width, int height, bool decorated, bool visible) :
		window(title, width, height, decorated, visible)
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
			//m_timestep.onUpdate();

			event_system.pollEvents();
			//onUpdate(m_timestep.getDeltaTime());
			onRender();

			graphic_context.swapBuffers();
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