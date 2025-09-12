export module bm.app;

import std;

import bm.log;

import bm.gfx.renderer;

import bm.window;
import bm.input;
import bm.cursor;

import bm.layer.stack;
import bm.layer.base;

import bm.event.base;
import bm.event.app;

import bm.taskqueue;

import bm.fps_limiter;

namespace bm 
{

export class Application
{

public:

	int run(int argc, char** argv);
	static Application& get() { return *s_app; }

	gfx::Renderer& getRenderer() { return m_renderer; }
	Window& getWindow() { return m_window; }

protected:

	Application() = delete;
	Application(std::string_view title, int width, int height, bool vsync = false, bool decorated = true, bool visible = true);
	virtual ~Application();

	virtual void processArgs(int argc, char** argv) {}

	virtual void onUpdate() {}
	virtual void onEvent(Event&) {};
	void onLayersEvent(Event& e);
	void onLayersUpdate();

	void pushLayer(LayerStack::ptr_t layer) { m_layers.pushLayer(layer); }
	void pushOverlay(LayerStack::ptr_t overlay) { m_layers.pushOverlay(overlay); }

	void registerEndOfFrameTask(TaskQueue::Task&& task) { m_end_of_frame_tasks.push(std::forward<TaskQueue::Task>(task)); }

	void close() { m_is_running = false; }
	bool isOpen() { return m_is_running; }

	void setFPSLimit(unsigned short fps_limit) { m_fps_limit = fps_limit; }

private:

	gfx::Renderer m_renderer;
	gfx::Context& m_ctx;

	LayerStack m_layers;

	Window m_window;

	TaskQueue m_end_of_frame_tasks;

	bool m_is_running;
	unsigned short m_fps_limit;

	static Application* s_app;
};

}

