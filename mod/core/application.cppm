export module bm.application;

import std;

import bm.log;

import bm.gfx.renderer;
import bm.gfx.context;

import bm.window;
import bm.window.manager;
import bm.input;
import bm.cursor;

import bm.layer.stack;
import bm.layer.base;

import bm.event.base;
import bm.event.app;

import bm.taskqueue;

namespace bm 
{

export template<typename WinManager = SingleWindowManager>
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
	//std::list<Window>& getWindows() { return m_windows; }
	
	//Main window is window that was added first
	bool isMainWindow() 
	{ 
		return isMainWindow(m_window_manager.getCurrent()); 
	}

	bool isMainWindow(Window& window) 
	{ 
		return *m_window_manager.begin() == window; 
	}

	//not actually destroy, just put in queue to destroy later (in end of frame)
	void closeWindow(Window& window)
	{
		m_end_of_frame_tasks.push([&window]() { m_window_manager.close(window); });
	}

	Window& openWindow(std::string_view title = "Bridge Maker App", int width = 1280, int height = 720, bool vsync = false, bool decorated = true, bool visible = true)
	{
		return m_window_manager.open(title, width, height, vsync, decorated, visible, m_ctx);
	}

protected:

	Application() : m_is_running(true), m_ctx(gfx::Context::getContext()) {}
	virtual ~Application() {};

	virtual void onUpdate() = 0;
	virtual void onEvent(Event&) = 0;
	void onLayersEvent(Event& e);

	void pushLayer(LayerStack::ptr_t layer) { m_layers.pushLayer(layer); }
	void pushOverlay(LayerStack::ptr_t overlay) { m_layers.pushOverlay(overlay); }

	void registerEndOfFrameTask(TaskQueue::Task&& task) { m_end_of_frame_tasks.push(std::forward<TaskQueue::Task>(task)); }

	void close() { m_is_running = false; }
	bool isOpen() { return m_is_running; }

private:

	gfx::Renderer m_renderer;
	gfx::Context& m_ctx;

	LayerStack m_layers;
	WinManager m_window_manager;
	TaskQueue m_end_of_frame_tasks;
	bool m_is_running;
};



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
	//processArgs(argc, argv);

	while (m_is_running)
	{
		for (auto& w : m_window_manager)
		{
			AppRenderEvent e;
			gfx::Context::makeCurrent(w);

			onUpdate();
			for (auto& l : m_layers)
				if (l->isEnabled())
					l->onUpdate();

			onEvent(e);

			gfx::Context::swapBuffers();
			w.onUpdate(); // poll events
		}
		m_end_of_frame_tasks.execute();
	}
	return 0;
}


}

