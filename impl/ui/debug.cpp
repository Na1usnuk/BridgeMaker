module bm.ui:debug;
import :debug;

import imgui;
import imgui_impl_glfw;
import imgui_impl_opengl3;

import bm.platform;

namespace bm::ui
{
	ImGuiRenderer::ImGuiRenderer(const Window& window)
	{
		ImGui::CheckVersion();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(window.getNative(), true);
		ImGui_ImplOpenGL3_Init();
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiRenderer::draw()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (auto& f : m_calls)
			f();
		m_calls.clear();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}