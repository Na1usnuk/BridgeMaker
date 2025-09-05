export module bm.application;

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

	template<typename App>
	static App& getApplication()
	{
		static App app;
		return app;
	}

	gfx::Renderer& getRenderer() { return m_renderer; }
	Window& getWindow() { return m_window; }

protected:

	Application() = delete;
	Application(std::string_view title, int width, int height, bool vsync = false, bool decorated = true, bool visible = true);
	virtual ~Application() {};

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
};


Application::Application(std::string_view title, int width, int height, bool vsync, bool decorated, bool visible) 
	: m_is_running(true), m_fps_limit(1000), m_ctx(gfx::Context::getContext()), m_window(title, width, height, vsync, decorated, visible)
{ 
	m_window.setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
}


void Application::onLayersUpdate()
{
	for (auto& l : m_layers)
		if (l->isEnabled())
			l->onUpdate();
}


void Application::onLayersEvent(Event& e)
{
	for (auto layer = m_layers.end(); layer > m_layers.begin();)
	{
		if (!(*--layer)->isEnabled()) continue;
		(*layer)->onEvent(e);
		if (e.isHandled())
			break;
	}
}


int Application::run(int argc, char** argv)
{
	processArgs(argc, argv);

	while (m_is_running)
	{
		FPSLimiter fps_limiter(m_fps_limit);

		onUpdate();

		AppRenderEvent e;

		m_window.onUpdate(); // poll events

		onLayersUpdate();

		onEvent(e);
		onLayersEvent(e);

		m_ctx.swapBuffers();

		m_end_of_frame_tasks.execute();
	}
	return 0;
}


}

