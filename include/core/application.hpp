#pragma once

#include "core.hpp"
#include "events/event.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"
#include "layer_stack.hpp"



#include "graphic/renderer.hpp"
#include "graphic/context.hpp"
#include "platform/window.hpp"
#include "platform/input.hpp"
#include "platform/cursor.hpp"
#ifdef BM_PLATFORM_X
	#include "graphic/opengl/opengl_renderer.hpp"
	#include "graphic/opengl/opengl_context.hpp"
	#include "platform/xplatform/xwindow.hpp"
	#include "platform/xplatform/xinput.hpp"
	#include "platform/xplatform/xcursor.hpp"
#elifdef BM_PLATFORM_WINDOWS 
	#error For now use crossplatform BM_PLATFORM_X                       
#elifdef BM_PLATFORM_LINUX
	#error For now use crossplatform BM_PLATFORM_X    
#elifdef BM_PLATFORM_MACOS
	#error For now use crossplatform BM_PLATFORM_X    
#endif


#include <string_view>
#include <list>
#include <utility>

BM_START

class Application
{
public:



public:

	int run(int argc, char** argv);

	template<typename App>
	static App& getApplication()
	{
		static App app;
		return app;
	}

	Renderer& getRenderer() { return m_renderer; }
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
	Renderer m_renderer;
	bool m_is_running = true;
	Window* m_close_window = nullptr;
	Context& m_ctx = Context::getContext();

};


BM_END

