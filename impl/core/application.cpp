module bm.app;

import std;

import bm.deltatime;

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


	void Application::onLayersUpdate(float delta_time)
	{
		for (auto& l : m_layers)
			if (l->isEnabled())
				l->onUpdate(delta_time);
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

		DeltaTime timestamp;

		while (m_is_running)
		{
			timestamp.setFPSLimit(m_fps_limit);
			timestamp.update();
			onUpdate(timestamp.getDeltaTime());

			AppRenderEvent e;

			m_window.onUpdate(); // poll events

			onLayersUpdate(timestamp.getDeltaTime());

			onEvent(e);
			onLayersEvent(e);

			m_ctx.swapBuffers();

			m_end_of_frame_tasks.execute();

			timestamp.wait();
		}
		return 0;
	}

}