#pragma once

#include "core.hpp"
#include "events/event.hpp"
#include "log.hpp"

#include <utility>
#include <string_view>
#include <functional>

#include "../deps/stb/stb_image.h"


struct GLFWwindow;

BM_START

	struct Icon
	{
		Icon(std::string_view path) : width(0), height(0), pixels(nullptr)
		{
			int bpp;
			pixels = stbi_load(path.data(), &width, &height, &bpp, 4);
			BM_CORE_ASSERT(pixels != nullptr, "Failed to load icon" + std::string(path.data()));
		}

		int width;
		int height;
		unsigned char* pixels;
	};

class XWindow
{
public:

	using NativeWindowPtr = GLFWwindow*;
	using EventCallbackFn = std::function<void(Event&)>;
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

	XWindow(const XWindow&) = delete;
	XWindow& operator=(const XWindow&) = delete;
	XWindow(XWindow&& oth) = delete;
	XWindow& operator=(XWindow&& oth) = delete;

	bool operator==(const XWindow& oth) const { return m_window == oth.m_window; }

	XWindow(std::string_view v, int w, int h, bool vs = false) : XWindow(Data{v, w, h, vs}) {}
	XWindow(const Data& data = Data{});
	~XWindow();

	void onUpdate();

	unsigned int		getWidth() const  { return m_data.width; }
	unsigned int		getHeight() const  { return m_data.height; }
	bool				getVSync() const  { return m_data.vsync; };
	NativeWindowPtr			getNativeWindow() const { return m_window; }
	std::pair<int, int> getPosition() const;

	void setVSync(bool enabled);
	void setEventCallback(EventCallbackFn fn) { m_data.callback = fn; }
	void setTitle(std::string_view title);
	void setIcon(std::string_view path);
	void setSize(int width, int height);
	void setOpacity(float opacity = 1.f);
	void setPosition(int x, int y);
	
	void resize(int width, int height);
	void hide();
	void show();

	bool isOpen() const;
	void close();
	void destroy();

	Window* getSelf() { return m_self; }

private:

	void create(const Data& props);
	void setGLFWPointer();
	void setKeyCallback();
	void setMuoseButtonCallback();
	void setCloseCallback();
	void setResizeCallback();
	void setPosCallback();
	void setAllCallbacks();
	void setMouseMoveCallback();

private:

	friend class AbstractWindow<XWindow>;

	GLFWwindow* m_window;
	Window* m_self;
	Data m_data;
};


BM_END