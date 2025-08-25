#pragma once

#include "event.hpp"

namespace bm {

class  MouseMoveEvent : public Event
{
public:
	
	MouseMoveEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

	float getX() const { return m_mouseX; }
	float getY() const { return m_mouseY; }

	BM_DEBUG_CODE(
	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseMoveEvent: " << m_mouseX << ", " << m_mouseY;
		return ss.str();
	}
	)

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:

	float m_mouseX, m_mouseY;

};


class  MouseScrollEvent : public Event
{
public:

	MouseScrollEvent(float x, float y) : m_offsetX(x), m_offsetY(y) {}

	float getX() const { return m_offsetX; }
	float getY() const { return m_offsetY; }

	BM_DEBUG_CODE(
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}
	)

		EVENT_CLASS_TYPE(MouseScrolled)
		
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:

	float m_offsetX, m_offsetY;
};


class  MouseButtonEvent : public Event
{
public:

	int getMouseButton() const { return m_button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

protected:

	MouseButtonEvent(int button) : m_button(button) {}

	int m_button;

};


class  MouseButtonPressedEvent : public MouseButtonEvent
{
public:

	MouseButtonPressedEvent(int button) :
		MouseButtonEvent(button) {
	}

	BM_DEBUG_CODE(
	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}
	)

	EVENT_CLASS_TYPE(MouseButtonPressed)

};


class  MouseButtonReleasedEvent : public MouseButtonEvent
{
public:

	MouseButtonReleasedEvent(int button) :
		MouseButtonEvent(button) {
	}

	BM_DEBUG_CODE(
	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}
	)

	EVENT_CLASS_TYPE(MouseButtonReleased)

};

}