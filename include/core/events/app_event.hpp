#pragma once

#include "event.hpp"

#include <utility>

namespace bm {

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
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

};


class WindowMoveEvent : public Event
{
public:
	WindowMoveEvent(int x, int y) : m_x(x), m_y(y) {}

	EVENT_CLASS_TYPE(WindowMoved)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

	int getX() const { return m_x; }
	int getY() const { return m_y; }
	std::pair<int, int> getPos() const { return { getX(), getY() }; }

private:

	int m_x, m_y;

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

}