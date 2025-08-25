#pragma once

#include "event.hpp"


namespace bm {


class  KeyEvent : public Event
{
public:

	int getKeyCode() const { return m_keyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
	KeyEvent(int keyCode) : m_keyCode(keyCode) { }

	int m_keyCode;
};


class  KeyPressedEvent : public KeyEvent
{
public:

	KeyPressedEvent(int keyCode, int repeatCount) : 
		KeyEvent(keyCode), m_repeatCount(repeatCount) {  }

	int getRepeatCount() const { return m_repeatCount; }

	BM_DEBUG_CODE(
	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
		return ss.str();
	}
	)

	EVENT_CLASS_TYPE(KeyPressed)

private:

	int m_repeatCount;
};


class  KeyReleasedEvent : public KeyEvent
{
public:

	KeyReleasedEvent(int keyCode) :
		KeyEvent(keyCode) {}

	BM_DEBUG_CODE(
		std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_keyCode;
		return ss.str();
	}
	)

	EVENT_CLASS_TYPE(KeyReleased)

private:

	int m_repeatCount;
};


}