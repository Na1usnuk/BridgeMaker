#pragma once

#include "core/core.hpp"


BM_START

struct WindowProps
{
	unsigned int width;
	unsigned int height;
	std::string title;

	WindowProps(unsigned int w = 1280, unsigned int h = 720, std::string t = "Bridge Maker App") : width(w), height(h), title(t) {}
};

class Window
{
public:
	
#ifdef BM_PLATFORM_WINDOWS 
	using Type = WindowsWindow;
#endif

	using EventCallbackFn = std::function<void(Event&)>;

public:

	virtual ~Window() {}

	virtual void onUpdate() = 0;

	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual bool getVSync() const = 0;

	virtual void setEventCallback(const EventCallbackFn& callback) = 0;
	virtual void setVSync(bool enabled) = 0;

	static std::unique_ptr<Window> create(const WindowProps& props = WindowProps());

};


BM_END