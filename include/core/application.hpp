#pragma once

#include "core.hpp"
#include "events/event.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"
#include "layer_stack.hpp"
#include "window.hpp"


//#define BM_USE_OPENGL //only opengl for now

#include "graphic/renderer.hpp"
#ifdef BM_USE_OPENGL
	#include "graphic/opengl/opengl_renderer.hpp"
	#define __BM_GRAPHIC_BACKEND ::BM::OpenGL::OpenGLRenderer
#elifdef BM_USE_VULCAN
	#error Vulcan not supported
#elifdef BM_USE_DIRECTX
	#error DirectX not supported
#elifdef BM_USE_METAL
	#error Metal not supported
#endif


#include <string_view>
#include <list>
#include <utility>

BM_START

class Application
{
public:

	using Renderer = Renderer<__BM_GRAPHIC_BACKEND>;

public:

	int run(int argc, char** argv);

protected:

	Application();
	virtual ~Application();

	virtual void onUpdate();
	virtual void onEvent(Event&);
	void onLayersEvent(Event& e);

	std::list<Window>& getWindows() { return m_windows; }

	void pushLayer(LayerStack::ptr_t layer) { m_layers.pushLayer(layer); }
	void pushOverlay(LayerStack::ptr_t overlay) { m_layers.pushOverlay(overlay); }

	//not actually destroy, just put in queue to destroy later (in end of frame)
	void closeWindow(Window* window);
	Window& addWindow(std::string_view title, int width, int height, bool vsync = false);

	Renderer& getRenderer() { return m_renderer; }

	void close() { m_is_running = false; }
	bool isOpen() { return m_is_running; }

private:

	//actually destroy window
	void _closeWindow(Window* window);

	LayerStack m_layers;
	std::list<Window> m_windows;
	Renderer m_renderer;
	bool m_is_running = true;
	Window* m_close_window = nullptr;

};


BM_END

