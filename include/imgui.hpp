#pragma once 

#include "core/core.hpp"
#include "core/layer.hpp"
#include "imgui.h"
#include "core/events/event.hpp"
#include "core/events/app_event.hpp"
#include "core/events/mouse_event.hpp"
#include "core/events/key_event.hpp"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


#include <vector>
#include <utility>
#include <functional>

namespace bm {


class ImGuiWindowRegistry
{
public:

	using WindowFn = std::function<void()>;
	using data_t = std::vector<WindowFn>;

public:

	ImGuiWindowRegistry() { m_window_queue.reserve(5); }

	void add(WindowFn fn);

	data_t::iterator begin() { return m_window_queue.begin(); }
	data_t::iterator end() { return m_window_queue.end(); }

	data_t::const_iterator begin() const { return m_window_queue.begin(); }
	data_t::const_iterator end() const { return m_window_queue.end(); }

private:

	data_t m_window_queue;
};

class ImGuiLayer : public Layer
{
public:

	using WindowFn = std::function<void()>;

public:

	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(const ImGuiLayer&) = delete;
	ImGuiLayer(ImGuiLayer&&) = default;
	ImGuiLayer& operator=(ImGuiLayer&&) = default;
	ImGuiLayer() = delete;
	ImGuiLayer(ImGuiWindowRegistry windows);
	~ImGuiLayer();

	void setCurrent(Window* window) override;

	void onUpdate() override;
	void onEvent(Event& e) override;
	void onAttach() override;
	void onDetach() override;

	bool onRender(bm::AppRenderEvent& e);

	bool onWindowClose(bm::WindowCloseEvent& e);
	bool onWindowMove(bm::WindowMoveEvent& e);
	bool onWindowResize(bm::WindowResizeEvent& e);
	bool onKeyPress(bm::KeyPressedEvent& e);
	bool onKeyRelease(bm::KeyReleasedEvent& e);
	bool onMouseRelease(bm::MouseButtonReleasedEvent& e);
	bool onMousePress(bm::MouseButtonPressedEvent& e);
	bool onMouseMove(bm::MouseMoveEvent& e);


private:

	ImGuiWindowRegistry m_windows;
	Window* m_window = nullptr;
};


}

