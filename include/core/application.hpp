#pragma once

#include "core.hpp"

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

	virtual int run(int argc, char** argv);
	
private:

	WindowsWindow m_window;
};


BM_END

