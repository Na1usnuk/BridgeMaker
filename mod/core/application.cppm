export module bm.application;

import std;

import bm.log;

import bm.gfx.renderer;

import bm.window;
import bm.window.manager;
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

export template<typename WinManager>
class Application
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
	WindowTraits<WinManager>::WindowContainer& getWindows() { return m_window_manager.windows(); }
	
	//Main window is window that was added first
	bool isMainWindow() 
	{ 
		return isMainWindow(m_ctx.getCurrent()); 
	}

	bool isMainWindow(Window& window) 
	{ 
		return *m_window_manager.windows()[0] == window;
	}

	//not actually destroy, just put in queue to destroy later (in end of frame)
	void closeWindow(Window& window)
	{
		m_end_of_frame_tasks.push([&window, this]() { this->m_window_manager.close(window); });
	}

	Window& openWindow(std::string_view title = "Bridge Maker App", int width = 1280, int height = 720, bool vsync = false, bool decorated = true, bool visible = true)
	{
		auto& window = m_window_manager.open(title, width, height, vsync, decorated, visible, m_ctx, m_end_of_frame_tasks);
		window.setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
		return window;
	}

protected:

	Application() : m_is_running(true), m_fps_limit(1000), m_ctx(gfx::Context::getContext()) {}
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

	WinManager m_window_manager;

	TaskQueue m_end_of_frame_tasks;

	bool m_is_running;
	unsigned short m_fps_limit;
};


template<typename WinManager>
void Application<WinManager>::onLayersUpdate()
{
	for (auto& l : m_layers)
		if (l->isEnabled())
			l->onUpdate();
}

template<typename WinManager>
void Application<WinManager>::onLayersEvent(Event& e)
{
	for (auto layer = m_layers.end(); layer > m_layers.begin();)
	{
		if (!(*--layer)->isEnabled()) continue;
		(*layer)->onEvent(e);
		if (e.isHandled())
			break;
	}
}

template<typename WinManager>
int Application<WinManager>::run(int argc, char** argv)
{
	processArgs(argc, argv);

	while (m_is_running)
	{
		FPSLimiter fps_limiter(m_fps_limit);

		onUpdate();

		for (auto& w : m_window_manager)
		{
			FPSLimiter fps_limit;

			AppRenderEvent e;
			e.setWindow((void*)&*w);
			m_ctx.makeCurrent(*w);

			w->onUpdate(); // poll events

			onLayersUpdate();

			onEvent(e);
			onLayersEvent(e);

			m_ctx.swapBuffers();
		}
		m_end_of_frame_tasks.execute();
	}
	return 0;
}



export using SingleWindowApp = Application<SingleWindowManager>;


}

