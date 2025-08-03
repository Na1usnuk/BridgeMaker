#pragma once

#include "core.hpp"
#include "events/event.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"

#ifdef BM_PLATFORM_WINDOWS
#define BM_API
#include "platform/windows/windows_window.hpp"
BM_START
using Window = WindowsWindow;
BM_END
#else 
#error "Windows only for now"
#endif 

#include <string_view>

BM_START

class Application
{
public:

	Application(std::string_view title, int width = 1280, int height = 720);
	virtual ~Application();

	virtual void onUpdate() const = 0;
	virtual void onEvent(Event&);
	virtual bool onClose(WindowCloseEvent& e);

	virtual bool onResize(WindowResizeEvent& e);


	virtual int run(int argc, char** argv);
	
private:

	WindowsWindow m_window;
};


BM_END

