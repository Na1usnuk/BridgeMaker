module bm.app;

import std;

namespace bm
{

	Application* Application::s_app = nullptr;

	Application::Application(std::string_view title, int width, int height, bool vsync, bool decorated, bool visible)
		: m_is_running(true), m_fps_limit(1000), m_ctx(gfx::Context::getContext()), m_window(title, width, height, vsync, decorated, visible)
	{
		s_app = this;
		m_window.setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
		m_renderer.setView({ 0, 0, width, height });
	}

	Application::~Application()
	{
		s_app = nullptr;
	}


	void Application::onLayersUpdate()
	{
		for (auto& l : m_layers)
			if (l->isEnabled())
				l->onUpdate();
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
		processArgs(argc, argv);

		while (m_is_running)
		{
			FPSLimiter fps_limiter(m_fps_limit);

			onUpdate();

			AppRenderEvent e;

			m_window.onUpdate(); // poll events

			onLayersUpdate();

			onEvent(e);
			onLayersEvent(e);

			m_ctx.swapBuffers();

			m_end_of_frame_tasks.execute();
		}
		return 0;
	}

}