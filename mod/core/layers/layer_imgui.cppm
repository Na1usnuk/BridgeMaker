export module bm.layer.imgui;

import std;

import bm.layer.base;
import bm.event;
import bm.window;

export import "imgui.h";
export import "imgui_impl_opengl3.h";
export import "imgui_impl_glfw.h";



namespace bm {


export class ImGuiWindowRegistry
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

export class ImGuiLayer : public Layer
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

	void setWindow(Window& window) override;

	void onUpdate(float delta_time) override;
	void onEvent(Event& e) override;
	void onAttach() override;
	void onDetach() override;

	bool onRender(bm::AppRenderEvent& e);

	bool onWindowClose(bm::WindowCloseEvent& e);
	bool onWindowMove(bm::WindowMoveEvent& e);
	bool onWindowResize(bm::WindowResizeEvent& e);
	bool onKeyPress(bm::KeyPressEvent& e);
	bool onKeyRelease(bm::KeyReleaseEvent& e);
	bool onMouseRelease(bm::MouseButtonReleaseEvent& e);
	bool onMousePress(bm::MouseButtonPressEvent& e);
	bool onMouseMove(bm::MouseMoveEvent& e);


private:

	ImGuiWindowRegistry m_windows;

};


}

