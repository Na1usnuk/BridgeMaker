export module bm.app;

import std;

import bm.log;

import bm.gfx.renderer;

import bm.window;
import bm.input;
import bm.cursor;

import bm.layer.stack;
import bm.layer.base;
import bm.layer.imgui;

import bm.event.base;
import bm.event.app;

import bm.taskqueue;


namespace bm 
{

export class Application
{

public:

	int run(int argc, char** argv);
	static Application& get() { return *s_app; }

	gfx::Renderer& getRenderer() { return m_renderer; }
	Window& getWindow() { return m_window; }
	gfx::Context& getContext() { return m_ctx; }

protected:

	Application() = delete;
	Application(std::string_view title, int width, int height, bool vsync = false, bool decorated = true, bool visible = true);
	virtual ~Application();

	virtual void processArgs(int argc, char** argv) {}

	virtual void onUpdate(float delta_time) {}
	virtual void onEvent(Event&) {}
	virtual void onImGuiRender() {}

	void pushLayer(LayerStack::ptr_t layer) { m_layers.pushLayer(layer); }
	void pushOverlay(LayerStack::ptr_t overlay) { m_layers.pushOverlay(overlay); }

	void registerEndOfFrameTask(TaskQueue::Task&& task) { m_end_of_frame_tasks.push(std::forward<TaskQueue::Task>(task)); }

	void close() { m_is_running = false; }
	bool isOpen() { return m_is_running; }

	void setFPSLimit(unsigned short fps_limit) { m_fps_limit = fps_limit; }

private:

	void onLayersEvent(Event& e);
	void onLayersUpdate(float delta_time);
	void onLayersImGuiRender();

	void onEventImpl(Event& e);
	void onUpdateImpl(float delta_time);
	void onImGuiRenderImpl();

private:

	gfx::Renderer m_renderer;

	LayerStack m_layers;
	std::weak_ptr<ImGuiLayer> m_imgui_layer;

	Window m_window;

	gfx::Context& m_ctx;

	TaskQueue m_end_of_frame_tasks;

	bool m_is_running;
	unsigned short m_fps_limit;

	static Application* s_app;
};

}

