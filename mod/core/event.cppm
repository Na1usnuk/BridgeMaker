export module bm.core:event;

import std;

namespace bm::event
{

	export class Event
	{
	public:

		enum class Type
		{
			NONE = 0,
			WindowClose, WindowResize, WindowLostFocus, WindowMove,
			AppTick, AppUpdate, AppRender,
			KeyPress, KeyRelease,
			MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
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

		virtual constexpr const char* getName() const = 0;
		virtual std::string toString() const { return getName(); }

		bool isInCategory(Category category) const { return getCategoryFlags() & category; }

		bool isHandled() const { return m_handled; }

	protected:

		friend class Dispatcher;

		bool m_handled = false;

	};


	export template<typename T, typename EVENT>
		constexpr std::function<bool(EVENT&)> bindEventFunc(bool(T::* func)(EVENT&), T* instance)
	{
		return [func, instance](EVENT& e) -> bool
			{
				return (instance->*func)(e);
			};
	}


	export class Dispatcher
	{
	private:

		template<typename EVENT>
		using EventFn = std::function<bool(EVENT&)>;

	public:

		Dispatcher(Event& event) : m_event(event) {}

		template<typename EVENT>
		bool dispatch(EventFn<EVENT> func)
		{
			if (m_event.getType() == EVENT::getStaticType())
			{
				m_event.m_handled = func(*(EVENT*)&m_event);
				return true;
			}
			return false;
		}

	private:

		Event& m_event;

	};


	export class  AppTick : public Event
	{
	public:

		AppTick() {}

		const char* getName() const override { return "AppTick"; }

		static Type getStaticType() { return Type::AppTick; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	};


	export class  AppUpdate : public Event
	{
	public:

		AppUpdate() {}

		const char* getName() const override { return "AppUpdate"; }

		static Type getStaticType() { return Type::AppUpdate; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	};


	export class  AppRender : public Event
	{
	public:

		AppRender() {}

		const char* getName() const override { return "AppRenderEvent"; }

		static Type getStaticType() { return Type::AppRender; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	};

}