export module bm.event.base;

import std;


export namespace bm
{


class EventDispatcher;

class Event
{
public:

	enum class Type
	{
		NONE = 0,
		WindowClose, WindowResize, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
	
	
	enum Category
	{
		None = 0,
		Application = 1 << 0,
		Input = 1 << 1,
		Keyboard = 1 << 2,
		Mouse = 1 << 3,
		MouseButton = 1 << 4,
	};

	using EventCallbackFn = std::function<void(Event&)>;

public:

	Event() {}

	virtual Type getType() const = 0;
	virtual int getCategoryFlags()  const = 0;

	virtual const char* getName() const = 0;
	virtual std::string toString() const { return getName(); }

	bool isInCategory(Category category) const
	{
		return getCategoryFlags() & category;
	}

	bool isHandled() const { return m_handled; }


protected:

	friend class EventDispatcher;

	bool m_handled = false;

};

template<typename T, typename INST>
auto bindEventFunc(T func, INST* instance)
{
	return std::bind(func, instance, std::placeholders::_1);
}

class EventDispatcher
{
	template<typename T>
	using EventFn = std::function<bool(T&)>;

public:

	EventDispatcher(Event& event) : m_event(event) {}

	template<typename T>
	bool dispatch(EventFn<T> func)
	{
		if (m_event.getType() == T::getStaticType())
		{
			m_event.m_handled = func(*(T*)&m_event);
			return true;
		}
		return false;
	}

private:

	Event& m_event;

};


inline std::ostream& operator<<(std::ostream& os, const Event& event)
{
	return os << event.toString();
}

}