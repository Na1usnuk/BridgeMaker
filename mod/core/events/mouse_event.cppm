export module bm.event.mouse;

import std;
import bm.config;
import bm.event.base;
import bm.input;

export namespace bm
{

class  MouseMoveEvent : public Event
{
public:

	MouseMoveEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

	float getX() const { return m_mouseX; }
	float getY() const { return m_mouseY; }

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "MouseMovedEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::MouseMoved; }
	Type getType() const override { return getStaticType(); }
	int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

private:

	float m_mouseX, m_mouseY;

};


class  MouseScrollEvent : public Event
{
public:

	MouseScrollEvent(float x, float y) : m_offsetX(x), m_offsetY(y) {}

	float getX() const { return m_offsetX; }
	float getY() const { return m_offsetY; }

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "MouseScrolledEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::MouseScrolled; }
	Type getType() const override { return getStaticType(); }
	int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

private:

	float m_offsetX, m_offsetY;
};


class  MouseButtonEvent : public Event
{
public:

	Input::Mouse getMouseButton() const { return m_button; }

	int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

protected:

	MouseButtonEvent(int button) : m_button(static_cast<Input::Mouse>(button)) {}

	Input::Mouse m_button;

};


class  MouseButtonPressedEvent : public MouseButtonEvent
{
public:

	MouseButtonPressedEvent(int button) :
		MouseButtonEvent(button) {
	}

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << static_cast<int>(m_button);
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "MouseButtonPressedEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::MouseButtonPressed; }
	Type getType() const override { return getStaticType(); }

};


class  MouseButtonReleasedEvent : public MouseButtonEvent
{
public:

	MouseButtonReleasedEvent(int button) :
		MouseButtonEvent(button) {
	}

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << static_cast<int>(m_button);
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "MouseButtonReleasedEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::MouseButtonReleased; }
	Type getType() const override { return getStaticType(); }

};

}