export module bm.app;

import std;

import bm.log;

import bm.gfx.renderer;
import bm.gfx.screen_renderer;

import bm.window;
import bm.input;
import bm.cursor;

import bm.layer.stack;
import bm.layer.base;
import bm.layer.imgui;

import bm.event.base;
import bm.event.app;

import bm.taskqueue;

import bm.traits;


namespace bm 
{

export class Application
{

public:

	int run(int argc, char** argv);
	static Application& get() { return *s_app; }

	gfx::Renderer& getRenderer() { return m_renderer; }
	gfx::ScreenRenderer& getScreenRenderer() { return m_screen_renderer; }
	Window& getWindow() { return m_window; }
	gfx::Context& getContext() { return m_ctx; }

protected:

	Application() = delete;
	Application(std::string_view title, int width, int height, bool vsync = false, bool decorated = true, bool visible = true);
	virtual ~Application();

	virtual void processArgs(int argc, char** argv) {}

	virtual void onUpdate(float delta_time) {}
	virtual void onEvent(Event& e);
	virtual void onImGuiRender() {}

	Traits<Layer>::OPtr pushLayer(Traits<Layer>::Ptr layer) { return m_layers.pushLayer(std::move(layer)); }
	Traits<Layer>::OPtr pushOverlay(Traits<Layer>::Ptr overlay) { return m_layers.pushOverlay(std::move(overlay)); }

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

	bool m_is_running;
	unsigned short m_fps_limit;

	Window m_window;
	gfx::Context& m_ctx;

	gfx::Renderer m_renderer;
	gfx::ScreenRenderer m_screen_renderer;

	LayerStack m_layers;
	Traits<ImGuiLayer>::OPtr m_imgui;

	TaskQueue m_end_of_frame_tasks;

	static Application* s_app;
};

}

