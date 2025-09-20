module bm.app;

import std;

import bm.deltatime;

import bm.layer.imgui;


namespace bm
{

	Application* Application::s_app = nullptr;

	Application::Application(std::string_view title, int width, int height, bool vsync, bool decorated, bool visible) :
		m_is_running(true),
		m_fps_limit(1000),
		m_window(title, width, height, vsync, decorated, visible),
		m_ctx(gfx::Context::getContext()),
		m_imgui(Layer::make<ImGuiLayer>())
	{
		m_layers.pushOverlay(m_imgui);

		s_app = this;
		m_window.setEventCallback(std::bind(&Application::onEventImpl, this, std::placeholders::_1));
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

	void Application::onLayersImGuiRender()
	{
		for (auto& l : m_layers)
			if (l->isEnabled())
				l->onImGuiRender();
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

	void Application::onImGuiRenderImpl()
	{
		m_imgui->Begin(); // Basic ImGui init like NewFrame, etc.
		onImGuiRender();
		onLayersImGuiRender(); // Layer overriden imgui window
		m_imgui->End();
	}

	void Application::onEventImpl(Event& e)
	{
		onEvent(e);
		onLayersEvent(e);
	}

	void Application::onUpdateImpl(float delta_time)
	{
		onUpdate(delta_time);
		onLayersUpdate(delta_time);
	}

	int Application::run(int argc, char** argv)
	{
		processArgs(argc, argv);

		DeltaTime timestamp;

		while (m_is_running)
		{
			timestamp.setFPSLimit(m_fps_limit);
			timestamp.update();

			onUpdateImpl(timestamp.getDeltaTime()); // Update all
			m_window.onUpdate(); // Poll events

			AppRenderEvent e;
			onEventImpl(e); // Every frame we render something, doesn`t we?

			onImGuiRenderImpl(); // ImGui render

			m_ctx.swapBuffers(); 

			m_end_of_frame_tasks.execute();

			timestamp.wait(); // Wait to match target FPS
		}
		return 0;
	}

}