module;

#include "../deps/stb/stb_image.h"
#include "events/event.hpp"

#include "GLFW/glfw3.h"

export module bm.window;

import std;


import bm.log;
import bm.assert;


namespace bm {

export struct Icon
{
	Icon(std::string_view path) : width(0), height(0), pixels(nullptr)
	{
		int bpp;
		pixels = stbi_load(path.data(), &width, &height, &bpp, 4);
		core::verify(pixels != nullptr, "Failed to load icon" + std::string(path.data()));
	}
	Icon(int w, int h, unsigned char* p) : width(w), height(h), pixels(p) {}

	int width;
	int height;
	unsigned char* pixels;
};

export class Window
{
public:

	using NativeWindowPtr = GLFWwindow*;
	using EventCallbackFn = std::function<void(Event&)>;
	using Icon = ::bm::Icon;
	struct Data
	{
		Data(std::string_view t = "BridgeMaker APP", int w = 1280, int h = 720, bool vs = false) : title(t), width(w), height(h), vsync(vs) {}
		std::string title;
		int width, height;
		bool vsync;
		EventCallbackFn callback;
		Window* window = nullptr;

		struct
		{
			int key;
			int repeat_count;
		}last_key;
	};


public:

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&& oth) = delete;
	Window& operator=(Window&& oth) = delete;

	bool operator==(const Window& oth) const { return m_window == oth.m_window; }

	Window(std::string_view v, int w, int h, bool vs = false, bool decorated = true, bool visible = true) : Window(Data{v, w, h, vs}, decorated, visible) {}
	Window(const Data& data = Data{}, bool decorated = true, bool visible = true);
	~Window();

	void onUpdate();

	int		            getWidth() const  { return m_data.width; }
	int		            getHeight() const  { return m_data.height; }
	bool				getVSync() const  { return m_data.vsync; };
	NativeWindowPtr		getNativeWindow() const { return m_window; }
	std::pair<int, int> getPosition() const;
	std::pair<int, int> getFramebufferSize() const;
	std::pair<int, int> getSize() const;
	std::pair<int, int> getFramebufferPosition() const;
	std::string			getTitle() const { return m_data.title; }

	void setVSync(bool enabled);
	void setEventCallback(EventCallbackFn fn) { m_data.callback = fn; }
	void setTitle(std::string_view title);
	void setIcon(Icon icon);
	void setSize(int width, int height);
	void setOpacity(float opacity = 1.f);
	void setPosition(int x, int y);
	
	void resize(int width, int height);
	void hide();
	void show();

	bool isOpen() const;
	void close();
	void destroy();

	static void onFocus(NativeWindowPtr window, int focused);

	Window* getSelf() { return m_self; }

private:

	void create(const Data& props, bool decorated, bool visible);
	void setGLFWPointer();
	void setKeyCallback();
	void setMuoseButtonCallback();
	void setCloseCallback();
	void setResizeCallback();
	void setPosCallback();
	void setAllCallbacks();
	void setMouseMoveCallback();
	void setWindowFocusCallback();

private:

	GLFWwindow* m_window;
	Window* m_self;
	Data m_data;
};


}