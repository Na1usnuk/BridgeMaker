#pragma once

#include "core.hpp"
#include "events/event.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"
#include "layer_stack.hpp"
#include "window.hpp"

#include <string_view>

BM_START

class Application
{
public:

	Application(std::string_view title, int width = 1280, int height = 720);
	virtual ~Application();

	virtual void onUpdate();
	virtual void onEvent(Event&);
	virtual bool onClose(WindowCloseEvent& e);
	virtual bool onResize(WindowResizeEvent& e);

	void pushLayer(LayerStack::raw_ptr_t layer) { m_layers.pushLayer(layer); }
	void pushOverlay(LayerStack::raw_ptr_t overlay) { m_layers.pushOverlay(overlay); }

	virtual int run(int argc, char** argv);
	
private:

	Window m_window;
	LayerStack m_layers;
	
};


BM_END

