export module bm.app:event;

import bm.platform;

import std;

namespace bm
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

		using EventCallback = std::function<void(Event&)>;

	public:

		Event() = default;
		virtual ~Event() = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = default;
		Event& operator=(Event&&) = default;

		virtual Type getType() const = 0;
		virtual int getCategoryFlags()  const = 0;

		virtual constexpr const char* getName() const = 0;
		virtual std::string toString() const { return getName(); }

		bool isInCategory(Category category) const { return getCategoryFlags() & category; }

		bool isHandled() const { return m_handled; }

	protected:

		friend class EventDispatcher;

		bool m_handled = false;

	};


	export class EventSystem
	{
	public:

		using EventCallback = std::function<void(Event&)>;

		using WindowResizeCallback = std::function<void(int, int)>;
		using WindowMoveCallback = std::function<void(int, int)>;
		using WindowCloseCallback = std::function<void(void)>;

		using KeyPressCallback = std::function<void(Input::Key, unsigned int)>;
		using KeyReleaseCallback = std::function<void(Input::Key)>;

		using MouseMoveCallback = std::function<void(double, double)>;
		using MouseButtonPressCallback = std::function<void(Input::Mouse)>;
		using MouseButtonReleaseCallback = std::function<void(Input::Mouse)>;
		using MouseScrollCallback = std::function<void(double, double)>;

		
	private:

		struct Callbacks
		{
			// By default every callback construct object of event and call universal callback
			Callbacks();

			EventCallback event;

			struct
			{
				WindowResizeCallback resize;
				WindowMoveCallback move;
				WindowCloseCallback close;
			} window;

			struct
			{
				Input::Key last;
				unsigned int repeat;

				KeyPressCallback press;
				KeyReleaseCallback release;
			} key;

			struct
			{
				MouseMoveCallback move;
				MouseButtonPressCallback press;
				MouseButtonReleaseCallback release;
				MouseScrollCallback scroll;
			} mouse;
		};

	public:

		explicit EventSystem(const Window& window) noexcept;
		~EventSystem();

		EventSystem(const EventSystem&) = delete;
		EventSystem& operator=(const EventSystem&) = delete;

		EventSystem(EventSystem&&) = default;
		EventSystem& operator=(EventSystem&&) = default;

		void setEventCallback(EventCallback callback) { m_callbacks.event = std::move(callback); }

		void setWindowResizeCallback(WindowResizeCallback callback) { m_callbacks.window.resize = std::move(callback); }
		void setWindowMoveCallback(WindowMoveCallback callback) { m_callbacks.window.move = std::move(callback); }
		void setWindowCloseCallback(WindowCloseCallback callback) { m_callbacks.window.close = std::move(callback); }

		void setKeyPressCallback(KeyPressCallback callback) { m_callbacks.key.press = std::move(callback); }
		void setKeyReleaseCallback(KeyReleaseCallback callback) { m_callbacks.key.release = std::move(callback); }
		
		void setMouseMoveCallback(MouseMoveCallback callback) { m_callbacks.mouse.move = std::move(callback); }
		void setMouseButtonPressCallback(MouseButtonPressCallback callback) { m_callbacks.mouse.press = std::move(callback); }
		void setMouseButtonReleaseCallback(MouseButtonReleaseCallback callback) { m_callbacks.mouse.release = std::move(callback); }
		void setMouseScrollCallback(MouseScrollCallback callback) { m_callbacks.mouse.scroll = std::move(callback); }

	private:

		const Window& m_window;

		Callbacks m_callbacks;
	};


	export template<typename T, typename EVENT>
		constexpr std::function<bool(EVENT&)> bindEventFunc(bool(T::* func)(EVENT&), T* instance)
	{
		return [func, instance](EVENT& e) -> bool
			{
				return (instance->*func)(e);
			};
	}


	export class EventDispatcher
	{
	private:

		template<typename EVENT>
		using EventFn = std::function<bool(EVENT&)>;

	public:

		EventDispatcher(Event& event) : m_event(event) {}

		template<typename EVENT>
		bool dispatch(EventFn<EVENT> func)
		{
			if (m_event.getType() == EVENT::getStaticType())
			{
				m_event.m_handled = func(static_cast<EVENT&>(m_event));
				return true;
			}
			return false;
		}

	private:

		Event& m_event;

	};

	// Application Events (dont use them, use onUpdate and onRender)

	export class  AppTickEvent : public Event
	{
	public:

		AppTickEvent() {}

		const char* getName() const override { return "AppTickEvent"; }

		static Type getStaticType() { return Type::AppTick; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	};


	export class  AppUpdateEvent : public Event
	{
	public:

		AppUpdateEvent() {}

		const char* getName() const override { return "AppUpdateEvent"; }

		static Type getStaticType() { return Type::AppUpdate; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	};


	export class  AppRenderEvent : public Event
	{
	public:

		AppRenderEvent() {}

		const char* getName() const override { return "AppRenderEvent"; }

		static Type getStaticType() { return Type::AppRender; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	};

	// Window Events

	export class  WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent(int x, int y) : m_width(x), m_height(y) {}

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		//std::string toString() const override;
		constexpr const char* getName() const override { return "WindowResizeEvent"; }

		static Type getStaticType() { return Type::WindowResize; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	private:

		int	m_height, m_width;

	};


	export class  WindowCloseEvent : public Event
	{
	public:

		constexpr const char* getName() const override { return "WindowCloseEvent"; }

		static Type getStaticType() { return Type::WindowClose; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

	};


	export class WindowMoveEvent : public Event
	{
	public:

		WindowMoveEvent(const int x, const int y) : m_x(x), m_y(y) {}

		//std::string toString() const override;
		constexpr const char* getName() const override { return "WindowMoveEvent"; }

		static Type getStaticType() { return Type::WindowMove; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Application; }

		const int getX() const { return m_x; }
		const int getY() const { return m_y; }
		std::pair<const int, const int> getPosition() const { return { getX(), getY() }; }

	private:

		const int m_x, m_y;

	};

	// Input Events

	export class  MouseMoveEvent : public Event
	{
	public:

		MouseMoveEvent(double x, double y) : m_x(x), m_y(y) {}

		double getX() const { return m_x; }
		double getY() const { return m_y; }

		//std::string toString() const override;

		const char* getName() const override { return "MouseMoveEvent"; }

		static Type getStaticType() { return Type::MouseMove; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

	private:

		double m_x, m_y;

	};


	export class  MouseScrollEvent : public Event
	{
	public:

		MouseScrollEvent(double x, double y) : m_x(x), m_y(y) {}

		double getX() const { return m_x; }
		double getY() const { return m_y; }
		std::pair<double, double> getOffset() const { return { m_x, m_y }; }

		//std::string toString() const override;
		const char* getName() const override { return "MouseScrollEvent"; }

		static Type getStaticType() { return Type::MouseScroll; }
		Type getType() const override { return getStaticType(); }
		int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

	private:

		double m_x, m_y;

	};


	class  MouseButtonEvent : public Event
	{
	public:

		Input::Mouse getButton() const { return m_button; }

		int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

	protected:

		MouseButtonEvent(::bm::Input::Mouse button) : m_button(button) {}

		Input::Mouse m_button;

	};


	export class  MouseButtonPressEvent : public MouseButtonEvent
	{
	public:

		MouseButtonPressEvent(::bm::Input::Mouse button) :
			MouseButtonEvent(button)
		{
		}

		//std::string toString() const override;
		const char* getName() const override { return "MouseButtonPressEvent"; }

		static Type getStaticType() { return Type::MouseButtonPress; }
		Type getType() const override { return getStaticType(); }

	};


	export class  MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:

		MouseButtonReleaseEvent(Input::Mouse button) :
			MouseButtonEvent(button)
		{
		}

		//std::string toString() const override;
		const char* getName() const override { return "MouseButtonReleaseEvent"; }

		static Type getStaticType() { return Type::MouseButtonRelease; }
		Type getType() const override { return getStaticType(); }

	};


	class  KeyEvent : public Event
	{
	public:

		Input::Key getKey() const { return m_key; }

		int getCategoryFlags() const override { return Category::Keyboard | Category::Input; }

	protected:

		KeyEvent(Input::Key key) : m_key(key) {}

		Input::Key m_key;

	};


	export class  KeyPressEvent : public KeyEvent
	{
	public:

		KeyPressEvent(Input::Key key, int repeat_count) :
			KeyEvent(key), m_repeat_count(repeat_count)
		{
		}

		int getRepeatCount() const { return m_repeat_count; }

		//std::string toString() const override;
		const char* getName() const override { return "KeyPressEvent"; }

		static Type getStaticType() { return Type::KeyPress; }
		Type getType() const override { return getStaticType(); }

	private:

		int m_repeat_count;

	};


	export class  KeyReleaseEvent : public KeyEvent
	{
	public:

		KeyReleaseEvent(Input::Key key) :
			KeyEvent(key)
		{
		}

		//std::string toString() const override;
		const char* getName() const override { return "KeyReleaseEvent"; }

		static Type getStaticType() { return Type::KeyRelease; }
		Type getType() const override { return getStaticType(); }

	};
}