module;

#include "../deps/stb/stb_image.h"
#include "GLFW/glfw3.h"

export module bm.window;

import std;


import bm.log;
import bm.assert;
import bm.event.base;


namespace bm {

namespace gfx
{
class Context;
}

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

	using NativeWindow = GLFWwindow*;
	using Icon = ::bm::Icon;
	struct Data
	{
		Data(std::string_view t, int w, int h, bool vs) : title(t), width(w), height(h), vsync(vs) {}
		std::string title;
		int width, height;
		bool vsync;
		Event::EventCallbackFn callback;
		Window* window = nullptr;

		struct
		{
			int key, repeat_count;
		} last_key;
	};

public:

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&& oth) = delete;
	Window& operator=(Window&& oth) = delete;

	bool operator==(const Window& oth) const { return m_window == oth.m_window; }

	Window(std::string_view v, int w, int h, bool vs, bool decorated, bool visible);
	~Window();

	void onUpdate();

	std::pair<int, int> getPosition() const;
	std::pair<int, int> getFramebufferSize() const;
	std::pair<int, int> getSize() const;
	std::pair<int, int> getFramebufferPosition() const;
	std::string	        getTitle() const { return m_data.title; }
	int                 getWidth() const { return m_data.width; }
	int	                getHeight() const { return m_data.height; }
	bool                getVSync() const { return m_data.vsync; };
	NativeWindow     getNativeWindow() const { return m_window; }

	void setVSync(bool enabled);
	void setTitle(std::string_view title);
	void setIcon(Icon icon);
	void setSize(int width, int height);
	void setOpacity(float opacity = 1.f);
	void setPosition(int x, int y);
	void setEventCallback(Event::EventCallbackFn fn) { m_data.callback = fn; }
	
	void resize(int width, int height);
	void hide();
	void show();

	bool isOpen() const;
	void close();
	void destroy();

private:

	void create(bool decorated, bool visible);
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
	Data m_data;
};


namespace gfx
{
	//window`s graphic context
	export class Context
	{
	public:

		Context() : m_window(nullptr) {}

		void init();
		void destroy();
		void makeCurrent(Window& window);
		Window& getCurrent();
		void swapBuffers();
		Window::NativeWindow shareContext();

		static Context& getContext();

	private:

		Window* m_window;

		static Context s_ctx_inst;

	};
}

}