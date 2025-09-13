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

	Input::Key getKey() const { return m_key; }

	int getCategoryFlags() const override { return Category::Keyboard | Category::Input; }

protected:

	KeyEvent(int key) : m_key(static_cast<Input::Key>(key)) {}

	Input::Key m_key;
};


class  KeyPressEvent : public KeyEvent
{
public:

	KeyPressEvent(int keyCode, int repeatCount) :
		KeyEvent(keyCode), m_repeatCount(repeatCount) {
	}

	int getRepeatCount() const { return m_repeatCount; }

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << static_cast<int>(m_key) << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "KeyPressEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::KeyPressed; }
	Type getType() const override { return getStaticType(); }

private:

	int m_repeatCount;
};


class  KeyReleaseEvent : public KeyEvent
{
public:

	KeyReleaseEvent(int keyCode) :
		KeyEvent(keyCode) {
	}

	std::string toString() const override
	{
		if constexpr (config::is_debug)
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << static_cast<int>(m_key);
			return ss.str();
		}
		else
			return "";
	}

	const char* getName() const override
	{
		if constexpr (config::is_debug)
			return "KeyReleaseEvent";
		else
			return "";
	}

	static Type getStaticType() { return Type::KeyReleased; }
	Type getType() const override { return getStaticType(); }

private:

	int m_repeatCount;
};


}