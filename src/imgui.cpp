#include "pch.hpp"

#include "imgui.hpp"

import bm.window;

namespace bm {

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
	m_window = window;
}

void ImGuiLayer::onUpdate()
{

}

bool ImGuiLayer::onRender(bm::AppRenderEvent& e)
{
	if (e.getWindow() != m_window)
		return false;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (auto& w : m_windows)
		w();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return false;
}

void ImGuiLayer::onEvent(Event& e)
{
	bm::EventDispatcher d(e);
	//log::core::info("Event occured in ImGuiLayer");

	d.dispatch<bm::WindowCloseEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onWindowClose));
	d.dispatch<bm::WindowMoveEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onWindowMove));
	d.dispatch<bm::WindowResizeEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onWindowResize));

	d.dispatch<bm::KeyPressedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onKeyPress));
	d.dispatch<bm::KeyReleasedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onKeyRelease));

	d.dispatch<bm::MouseButtonPressedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onMousePress));
	d.dispatch<bm::MouseButtonReleasedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onMouseRelease));
	d.dispatch<bm::MouseMoveEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onMouseMove));

	d.dispatch<bm::AppRenderEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onRender));
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

bool ImGuiLayer::onWindowClose(bm::WindowCloseEvent& e)
{
	return false;
}

bool ImGuiLayer::onWindowMove(bm::WindowMoveEvent& e)
{
	return false;
}

bool ImGuiLayer::onWindowResize(bm::WindowResizeEvent& e)
{
	return false;
}

bool ImGuiLayer::onKeyPress(bm::KeyPressedEvent& e)
{
	return false;
}

bool ImGuiLayer::onKeyRelease(bm::KeyReleasedEvent& e)
{
	return false;
}

bool ImGuiLayer::onMouseRelease(bm::MouseButtonReleasedEvent& e)
{
	if (e.getWindow() != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.getMouseButton()] = false;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMousePress(bm::MouseButtonPressedEvent& e)
{
	if (e.getWindow() != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.getMouseButton()] = true;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMouseMove(bm::MouseMoveEvent& e)
{
	if (e.getWindow() != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e.getX(), e.getY());

	return ImGui::IsAnyItemHovered();
}


}
