module bm.app;

import std;

import bm.event.app;
import bm.layer.imgui;


namespace bm
{

	Application* Application::s_app = nullptr;

	Application::Application(std::string_view title, int width, int height, bool vsync, bool decorated, bool visible) :
		m_is_running(true),
		m_window(title, width, height, vsync, decorated, visible),
		m_ctx(gfx::Context::get())
	{
		s_app = this;

		m_imgui = m_layers.pushOverlay(Layer::make<ImGuiLayer>());

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

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e)
			{
				this->close();
				return true;
			});
	}

	void Application::onImGuiRender()
	{
		ImGui::Begin("Application");

		ImGui::Text("Application running...");

		ImGui::Separator();

		ImGui::Text("Window title: %s", m_window.getTitle().c_str());
		ImGui::Text("Window size: %d x %d", m_window.getWidth(), m_window.getHeight());
		ImGui::Text("Texture slots: %d", m_renderer.getTextureSlotCount());


		ImGui::Separator();

		ImGui::Text("FPS: %.2f", m_timestep.getFPS());
		ImGui::Text("Frame time: %.3f ms", m_timestep.getDeltaTime() * 1000.0f);


		ImGui::End();
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

		while (m_is_running)
		{
			m_timestep.onUpdate();

			onUpdateImpl(m_timestep.getDeltaTime()); // Update all
			m_window.onUpdate(); // Poll events

			AppRenderEvent e;
			onEventImpl(e); // Every frame we render something, doesn`t we?

			onImGuiRenderImpl(); // ImGui render

			m_ctx.swapBuffers(); 

			m_end_of_frame_tasks.execute();
		}

		return 0;
	}

}