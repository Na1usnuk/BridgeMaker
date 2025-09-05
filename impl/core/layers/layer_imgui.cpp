module;

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

module bm.layer.imgui;

import bm.window;
import bm.event;
import bm.log;
import bm.input;

namespace bm {

ImGuiLayer::ImGuiLayer(ImGuiWindowRegistry windows) : m_windows(std::move(windows))
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::setCurrent(Window& window)
{
	ImGui_ImplGlfw_InitForOpenGL(window.getNativeWindow(), true);
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	m_window = &window;
}

void ImGuiLayer::onUpdate()
{

}

bool ImGuiLayer::onRender(bm::AppRenderEvent& e)
{
	if (getEventWindow(e) != *m_window)
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

	d.dispatch<bm::WindowCloseEvent>(bm::bindEventFunc(&ImGuiLayer::onWindowClose, this));
	d.dispatch<bm::WindowMoveEvent>(bm::bindEventFunc(&ImGuiLayer::onWindowMove, this));
	d.dispatch<bm::WindowResizeEvent>(bm::bindEventFunc(&ImGuiLayer::onWindowResize, this));

	d.dispatch<bm::KeyPressedEvent>(bm::bindEventFunc(&ImGuiLayer::onKeyPress, this));
	d.dispatch<bm::KeyReleasedEvent>(bm::bindEventFunc(&ImGuiLayer::onKeyRelease, this));

	d.dispatch<bm::MouseButtonPressedEvent>(bm::bindEventFunc(&ImGuiLayer::onMousePress, this));
	d.dispatch<bm::MouseButtonReleasedEvent>(bm::bindEventFunc(&ImGuiLayer::onMouseRelease, this));
	d.dispatch<bm::MouseMoveEvent>(bm::bindEventFunc(&ImGuiLayer::onMouseMove, this));

	d.dispatch<bm::AppRenderEvent>(bm::bindEventFunc(&ImGuiLayer::onRender, this));
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
	if (&getEventWindow(e) != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[static_cast<int>(e.getMouseButton())] = false;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMousePress(bm::MouseButtonPressedEvent& e)
{
	if (&getEventWindow(e) != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[static_cast<int>(e.getMouseButton())] = true;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMouseMove(bm::MouseMoveEvent& e)
{
	if (&getEventWindow(e) != m_window) return false;
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e.getX(), e.getY());

	return ImGui::IsAnyItemHovered();
}


}
