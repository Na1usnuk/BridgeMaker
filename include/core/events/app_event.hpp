#pragma once

#include "event.hpp"
#include "window.hpp"

BM_START

class  WindowResizeEvent : public Event
{
public:

	WindowResizeEvent(int x, int y) : m_width(x), m_height(y) {}

	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }

	BM_DEBUG_CODE(
	std::string toString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}
	)

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:

	int	m_height, m_width;

};


class  WindowCloseEvent : public Event
{
public:
	WindowCloseEvent(Window* w) : window(w) {}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

	Window* window; // window that throw event
};


class  AppTickEvent : public Event
{
public:
	AppTickEvent() {}

	EVENT_CLASS_TYPE(AppTick)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};


class  AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() {}

	EVENT_CLASS_TYPE(AppUpdate)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};


class  AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}

	EVENT_CLASS_TYPE(AppRender)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

BM_END