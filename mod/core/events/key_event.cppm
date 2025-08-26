export module bm.event.key;

import std;
import bm.config;
import bm.event.base;
import bm.input;

export namespace bm
{

class  KeyEvent : public Event
{
public:

	Input::Key getKeyCode() const { return m_keyCode; }

	int getCategoryFlags() const override { return Category::Keyboard | Category::Input; }

protected:

	KeyEvent(int keyCode) : m_keyCode(static_cast<Input::Key>(keyCode)) {}

	Input::Key m_keyCode;
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
			ss << "KeyPressedEvent: " << static_cast<int>(m_keyCode) << " (" << m_repeatCount << " repeats)";
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
			ss << "KeyReleasedEvent: " << static_cast<int>(m_keyCode);
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