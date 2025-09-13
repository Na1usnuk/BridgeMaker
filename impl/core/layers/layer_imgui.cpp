module;

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

module bm.layer.imgui;

import bm.window;
import bm.event;
import bm.log;
import bm.input;

import bm.layer.base;

import std;

namespace bm {

ImGuiLayer::ImGuiLayer(ImGuiWindowRegistry windows) : m_windows(std::move(windows))
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::setWindow(Window& window)
{
	ImGui_ImplGlfw_InitForOpenGL(window.getNativeWindow(), true);
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

void ImGuiLayer::onUpdate(float delta_time)
{

}

bool ImGuiLayer::onRender(bm::AppRenderEvent& e)
{
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

	d.dispatch<bm::WindowCloseEvent>(bm::bindEventFn(&ImGuiLayer::onWindowClose, this));
	d.dispatch<bm::WindowMoveEvent>(bm::bindEventFn(&ImGuiLayer::onWindowMove, this));
	d.dispatch<bm::WindowResizeEvent>(bm::bindEventFn(&ImGuiLayer::onWindowResize, this));

	d.dispatch<bm::KeyPressEvent>(bm::bindEventFn(&ImGuiLayer::onKeyPress, this));
	d.dispatch<bm::KeyReleaseEvent>(bm::bindEventFn(&ImGuiLayer::onKeyRelease, this));

	d.dispatch<bm::MouseButtonPressEvent>(bm::bindEventFn(&ImGuiLayer::onMousePress, this));
	d.dispatch<bm::MouseButtonReleaseEvent>(bm::bindEventFn(&ImGuiLayer::onMouseRelease, this));
	d.dispatch<bm::MouseMoveEvent>(bm::bindEventFn(&ImGuiLayer::onMouseMove, this));

	d.dispatch<bm::AppRenderEvent>(bm::bindEventFn(&ImGuiLayer::onRender, this));
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

bool ImGuiLayer::onKeyPress(bm::KeyPressEvent& e)
{
	return false;
}

bool ImGuiLayer::onKeyRelease(bm::KeyReleaseEvent& e)
{
	return false;
}

bool ImGuiLayer::onMouseRelease(bm::MouseButtonReleaseEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[static_cast<int>(e.getMouseButton())] = false;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMousePress(bm::MouseButtonPressEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[static_cast<int>(e.getMouseButton())] = true;

	return ImGui::IsAnyItemHovered();
}

bool ImGuiLayer::onMouseMove(bm::MouseMoveEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e.getX(), e.getY());

	return ImGui::IsAnyItemHovered();
}


}
