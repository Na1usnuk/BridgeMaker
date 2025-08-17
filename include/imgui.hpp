#pragma once 

#include "core/core.hpp"
#include "core/layer.hpp"
#include "imgui.h"
#include "core/events/event.hpp"
#include "core/events/app_event.hpp"
#include "core/events/mouse_event.hpp"
#include "core/events/key_event.hpp"

#ifdef BM_USE_OPENGL
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#endif // BM_USE_OPENGL


#include <vector>
#include <utility>
#include <functional>

BM_START


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

	bool onRender(BM::AppRenderEvent& e);

	bool onWindowClose(BM::WindowCloseEvent& e);
	bool onWindowMove(BM::WindowMoveEvent& e);
	bool onWindowResize(BM::WindowResizeEvent& e);
	bool onKeyPress(BM::KeyPressedEvent& e);
	bool onKeyRelease(BM::KeyReleasedEvent& e);
	bool onMouseRelease(BM::MouseButtonReleasedEvent& e);
	bool onMousePress(BM::MouseButtonPressedEvent& e);
	bool onMouseMove(BM::MouseMoveEvent& e);


private:

	ImGuiWindowRegistry m_windows;
	Window* m_window = nullptr;
};


BM_END

