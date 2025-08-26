export module bm.event.app;

import std;
import bm.config;
import bm.event.base;

export namespace bm
{

class  WindowResizeEvent : public Event
{
public:

	WindowResizeEvent(int x, int y) : m_width(x), m_height(y) {}

	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }


	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_width << ", " << m_height;
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override 
	{ 
		if constexpr (config::is_debug)
			return "WindowResizeEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::WindowResize; }
	Type getType() const override { return getStaticType(); } 
	int getCategoryFlags() const override { return Category::Application; }

private:

	int	m_height, m_width;

};


class  WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "WindowCloseEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::WindowClose; }
	Type getType() const override { return getStaticType(); }
	int getCategoryFlags() const override { return Category::Application; }

};


class WindowMoveEvent : public Event
{
public:
	WindowMoveEvent(int x, int y) : m_x(x), m_y(y) {}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "WindowMovedEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::WindowMoved; }
	Type getType() const override { return getStaticType(); }
	int getCategoryFlags() const override { return Category::Application; }

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

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "AppTickEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::AppTick; }
	Type getType() const override { return getStaticType(); }
	int getCategoryFlags() const override { return Category::Application; }
};


class  AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() {}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "AppUpdateEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::AppUpdate; }
	Type getType() const override { return getStaticType(); }
	int getCategoryFlags() const override { return Category::Application; }
};


class  AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "AppRenderEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::AppRender; }
	Type getType() const override { return getStaticType(); }
	int getCategoryFlags() const override { return Category::Application; }
};

}