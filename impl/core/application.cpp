module bm.app;

import std;

import bm.event.app;
import bm.layer.imgui;
import bm.config;


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
		if constexpr (config::enable_imgui)
		{
			for (auto& l : m_layers)
				if (l->isEnabled())
					l->onImGuiRender();
		}
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
		if constexpr (config::enable_imgui)
		{
			ImGui::Begin("Application");

			ImGui::Text("Application running...");

			ImGui::Separator();

			ImGui::Text("Window title: %s", m_window.getTitle().c_str());
			ImGui::Text("Window size: %d x %d", m_window.getWidth(), m_window.getHeight());
			static bool cursor_captured = false;
			if (ImGui::Checkbox("Capture Cursor", &cursor_captured))
				getWindow().setCaptureCursor(cursor_captured);
			ImGui::Text("Texture slots: %d", m_renderer.getTextureSlotCount());


			ImGui::Separator();

			ImGui::Text("FPS: %.2f", m_timestep.getFPS());
			ImGui::Text("Frame time: %.3f ms", m_timestep.getDeltaTime() * 1000.0f);

			if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// Background color
				static float background_color[3] = {0.f, 0.f, 0.f};
				if (ImGui::ColorEdit3("Background Color", background_color))
				{
					getRenderer().setBackgroundColor({ background_color[0], background_color [1], background_color [2], 1.f});
				}

				// Rendering flags
				ImGui::SeparatorText("Rendering");

				static bool wireframe = false;
				static bool depthTest = true;
				static bool blending = true;

				if (ImGui::Checkbox("Wireframe Mode", &wireframe))
					getRenderer().setPolygonMode(
						wireframe ? ::bm::gfx::Renderer::PolygonMode::Line
						: ::bm::gfx::Renderer::PolygonMode::Fill
					);

				if (ImGui::Checkbox("Depth Test", &depthTest))
					getRenderer().setDepthTesting(depthTest);

				if (ImGui::Checkbox("Blending", &blending))
					getRenderer().setBlend(blending);

				// Exposure (useful future param)
				static float exposure = 1.0f;
				ImGui::SliderFloat("Exposure", &exposure, 0.1f, 4.0f);

				// Scene summary
			}


			ImGui::End();
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