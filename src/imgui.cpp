#include "pch.hpp"

#include "imgui.hpp"

#include "platform/window.hpp"
#include "platform/xplatform/xwindow.hpp"

BM_START

ImGuiLayer::ImGuiLayer(ImGuiWindowRegistry windows) : m_windows(std::move(windows))
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::setCurrent(Window* window)
{
	ImGui_ImplGlfw_InitForOpenGL(window->getNativeWindow(), true);
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

void ImGuiLayer::onUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (auto& w : m_windows)
		w();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::onEvent(Event& e)
{
}

void ImGuiLayer::onAttach()
{
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
}

void ImGuiLayer::onDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiWindowRegistry::add(WindowFn fn)
{
	m_window_queue.push_back(fn);
}

BM_END
