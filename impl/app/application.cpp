module bm.app;

import bm.core;

namespace bm
{
	Application::Application(std::string_view title, int width, int heigth, bool vsync, bool decorated, bool visible) :
		m_window(title, width, heigth, vsync, decorated, visible),
		m_ctx(m_window),
		m_is_running(true),
		m_return_code(0)
	{
		m_window.setEventCallback([this](event::Event& e) { onEvent(e); });
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
			m_timestep.onUpdate();

			m_window.onUpdate(); // Poll events
			onUpdate(m_timestep.getDeltaTime());
			onRender();

			m_ctx.swapBuffers();
		}

		onShutdown();

		return m_return_code;
	}

	void Application::onEvent(event::Event& e)
	{
		event::Dispatcher d(e);
		d.dispatch<event::WindowClose>([this](event::WindowClose& c) {close(); return true; });
	}
}