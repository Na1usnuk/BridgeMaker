#pragma once

#include "core.hpp"
#include "events/event.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"
#include "layer_stack.hpp"

import std;

import bm.log;

import bm.gfx.renderer;
import bm.gfx.context;

import bm.window;
import bm.input;
import bm.cursor;

namespace bm 
{

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
	std::list<Window>& getWindows() { return m_windows; }
	//Main window is window that was added first
	bool isMainWindow(Window* window) { return &m_windows.front() == window; }
	//not actually destroy, just put in queue to destroy later (in end of frame)
	void closeWindow(Window* window);
	Window& addWindow(std::string_view title, int width, int height, bool vsync = false, bool decorated = true, bool visible = true);

protected:

	Application();
	virtual ~Application();

	virtual void onUpdate() = 0;
	virtual void onEvent(Event&) = 0;
	void onLayersEvent(Event& e);

	void pushLayer(LayerStack::ptr_t layer) { m_layers.pushLayer(layer); }
	void pushOverlay(LayerStack::ptr_t overlay) { m_layers.pushOverlay(overlay); }

	void close() { m_is_running = false; }
	bool isOpen() { return m_is_running; }

private:


	//actually destroy window
	void _closeWindow(Window* window);

	LayerStack m_layers;
	std::list<Window> m_windows;
	gfx::Renderer m_renderer;
	bool m_is_running = true;
	Window* m_close_window = nullptr;
	gfx::Context& m_ctx = gfx::Context::getContext();

};


}

