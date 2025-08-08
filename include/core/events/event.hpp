#pragma once

#include "core.hpp"

BM_START


enum class EventType
{
	NONE = 0,
	WindowClose, WindowResize, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};


enum EventCategory
{
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput       = BIT(1),
	EventCategoryKeyboard    = BIT(2),
	EventCategoryMouse       = BIT(3),
	EventCategoryMouseButton = BIT(4)

};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type;} \
							   virtual EventType getEventType() const override { return getStaticType(); } \
							   BM_DEBUG_CODE(virtual const char* getName() const override { return #type; }) 
							   
#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

class EventDispatcher;

class Event
{

public:

	virtual EventType getEventType() const = 0;
	virtual int getCategoryFlags()  const = 0;

	BM_DEBUG_CODE(
	virtual const char* getName() const = 0;
	virtual std::string toString() const { return getName(); }
	)

	bool isInCategory(EventCategory category) const
	{
		return getCategoryFlags() & category;
	}

	bool isHandled() const { return m_handled; }

protected:

	friend class EventDispatcher;

	bool m_handled = false;

};


class EventDispatcher
{
	template<typename T> 
	using EventFn = std::function<bool(T&)>;

public:

	EventDispatcher(Event& event) : m_event(event) {}

	template<typename T>
	bool dispatch(EventFn<T> func)
	{
		if (m_event.getEventType() == T::getStaticType())
		{
			m_event.m_handled = func(*(T*)&m_event);
			return true;
		}
		return false;
	}

private:

	Event& m_event;

};


BM_DEBUG_CODE(
inline std::ostream& operator<<(std::ostream& os, const Event& event)
{
	return os << event.toString();
}
)
BM_END