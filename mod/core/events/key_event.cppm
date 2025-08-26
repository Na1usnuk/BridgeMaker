export module bm.event.key;

import std;
import bm.config;
import bm.event.base;

export namespace bm
{

class  KeyEvent : public Event
{
public:

	int getKeyCode() const { return m_keyCode; }

	int getCategoryFlags() const override { return Category::Keyboard | Category::Input; }

protected:

	KeyEvent(int keyCode) : m_keyCode(keyCode) {}

	int m_keyCode;
};


class  KeyPressedEvent : public KeyEvent
{
public:

	KeyPressedEvent(int keyCode, int repeatCount) :
		KeyEvent(keyCode), m_repeatCount(repeatCount) {
	}

	int getRepeatCount() const { return m_repeatCount; }

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "KeyPressedEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::KeyPressed; }
	Type getType() const override { return getStaticType(); }

private:

	int m_repeatCount;
};


class  KeyReleasedEvent : public KeyEvent
{
public:

	KeyReleasedEvent(int keyCode) :
		KeyEvent(keyCode) {
	}

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "KeyReleasedEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::KeyReleased; }
	Type getType() const override { return getStaticType(); }

private:

	int m_repeatCount;
};


}