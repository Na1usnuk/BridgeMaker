#pragma once

#include "core.hpp"
#include "events/event.hpp"

struct GLFWwindow;

BM_START


class Window
{
public:

	using EventCallbackFn_t = std::function<void(Event&)>;
	struct Data
	{
		Data(std::string_view t = "BridgeMaker APP", int w = 1280, int h = 720, bool vs = false) : title(t), width(w), height(h), vsync(vs) {}
		std::string title;
		int width, height;
		bool vsync;
		EventCallbackFn_t callback;
	};

public:

	 Window(const Data& data = Data{});
	~Window();

	void onUpdate();

	unsigned int getWidth() const  { return m_data.width; }
	unsigned int getHeight() const  { return m_data.height; }
	bool         getVSync() const  { return m_data.vsync; };

	void setVSync(bool enabled);
	void setEventCallback(EventCallbackFn_t fn) { m_data.callback = fn; }

	void resize(int width, int height);
	void init(const Data& props);

	bool isOpen() const;
	void close();

private:

	GLFWwindow* m_window;
	Data m_data;
};


BM_END