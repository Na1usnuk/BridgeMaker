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
	m_window = window;
}

void ImGuiLayer::onUpdate()
{

}

bool ImGuiLayer::onRender(BM::AppRenderEvent& e)
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
	BM::EventDispatcher d(e);
	//BM_CORE_INFO("Event occured in ImGuiLayer");

	d.dispatch<BM::WindowCloseEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onWindowClose));
	d.dispatch<BM::WindowMoveEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onWindowMove));
	d.dispatch<BM::WindowResizeEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onWindowResize));

	d.dispatch<BM::KeyPressedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onKeyPress));
	d.dispatch<BM::KeyReleasedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onKeyRelease));

	d.dispatch<BM::MouseButtonPressedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onMousePress));
	d.dispatch<BM::MouseButtonReleasedEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onMouseRelease));
	d.dispatch<BM::MouseMoveEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onMouseMove));

	d.dispatch<BM::AppRenderEvent>(BM_BIND_EVENT_FN(ImGuiLayer::onRender));
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

bool ImGuiLayer::onWindowClose(BM::WindowCloseEvent& e)
{
	return false;
}

bool ImGuiLayer::onWindowMove(BM::WindowMoveEvent& e)
{
	return false;
}

bool ImGuiLayer::onWindowResize(BM::WindowResizeEvent& e)
{
	return false;
}

bool ImGuiLayer::onKeyPress(BM::KeyPressedEvent& e)
{
	return false;
}

bool ImGuiLayer::onKeyRelease(BM::KeyReleasedEvent& e)
{
	return false;
}

bool ImGuiLayer::onMouseRelease(BM::MouseButtonReleasedEvent& e)
{
	if (e.getWindow() != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.getMouseButton()] = false;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMousePress(BM::MouseButtonPressedEvent& e)
{
	if (e.getWindow() != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.getMouseButton()] = true;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMouseMove(BM::MouseMoveEvent& e)
{
	if (e.getWindow() != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e.getX(), e.getY());

	return ImGui::IsAnyItemHovered();
}


BM_END
