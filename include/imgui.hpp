#pragma once 

#include "core/core.hpp"
#include "core/layer.hpp"
#include "imgui.h"
#include "core/events/event.hpp"

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


private:

	ImGuiWindowRegistry m_windows;

};


BM_END

