export module bm.platform:input;

import std;
import bm.core;

import :window;

namespace bm
{

	export class Input
	{
	public:
	
		enum class Key;
		enum class Mouse;
	
	public:
	
		static bool isPressed(Key key);
		static bool isReleased(Key key);
		static bool isPressed(Mouse key);
		static bool isReleased(Mouse key);
		static std::pair<float, float> getMousePosition();
		static void setMousePosition(const std::pair<float, float>& pos);
	
		static Window* getCurrentWindow() { return s_window; }
		static void    setCurrentWindow(Window* window) { s_window = window; }
	
	private:
	
		static bool isPressedImpl(Window& window, int key);
		static bool isReleasedImpl(Window& window, int key);
	
		static Window* s_window;
	
		static std::pair<float, float> s_mouse_pos;
	
	};

	namespace event
	{
		export class  MouseMove : public Event
		{
		public:

			MouseMove(float x, float y) : m_mouseX(x), m_mouseY(y) {}

			float getX() const { return m_mouseX; }
			float getY() const { return m_mouseY; }

			std::string toString() const override;

			const char* getName() const override { return "MouseMovedEvent"; }

			static Type getStaticType() { return Type::MouseMove; }
			Type getType() const override { return getStaticType(); }
			int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

		private:

			float m_mouseX, m_mouseY;

		};


		export class  MouseScroll : public Event
		{
		public:

			MouseScroll(float x, float y) : m_offsetX(x), m_offsetY(y) {}

			float getX() const { return m_offsetX; }
			float getY() const { return m_offsetY; }

			std::string toString() const override;
			const char* getName() const override { return "MouseScroll"; }

			static Type getStaticType() { return Type::MouseScroll; }
			Type getType() const override { return getStaticType(); }
			int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

		private:

			float m_offsetX, m_offsetY;

		};


		class  MouseButton : public Event
		{
		public:

			::bm::Input::Mouse getKey() const { return m_button; }

			int getCategoryFlags() const override { return Category::Mouse | Category::Input; }

		protected:

			MouseButton(::bm::Input::Mouse button) : m_button(button) {}

			::bm::Input::Mouse m_button;

		};


		export class  MouseButtonPress : public MouseButton
		{
		public:

			MouseButtonPress(::bm::Input::Mouse button) :
				MouseButton(button)
			{}

			std::string toString() const override;
			const char* getName() const override { return "MouseButtonPress"; }

			static Type getStaticType() { return Type::MouseButtonPress; }
			Type getType() const override { return getStaticType(); }

		};


		export class  MouseButtonRelease : public MouseButton
		{
		public:

			MouseButtonRelease(::bm::Input::Mouse button) :
				MouseButton(button)
			{}

			std::string toString() const override;
			const char* getName() const override { return "MouseButtonRelease"; }

			static Type getStaticType() { return Type::MouseButtonRelease; }
			Type getType() const override { return getStaticType(); }

		};


		class  Key : public Event
		{
		public:

			::bm::Input::Key getKey() const { return m_key; }

			int getCategoryFlags() const override { return Category::Keyboard | Category::Input; }

		protected:

			Key(::bm::Input::Key key) : m_key(key) {}

			::bm::Input::Key m_key;

		};


		export class  KeyPress : public Key
		{
		public:

			KeyPress(::bm::Input::Key key, int repeatCount) :
				Key(key), m_repeatCount(repeatCount)
			{}

			int getRepeatCount() const { return m_repeatCount; }

			std::string toString() const override;
			const char* getName() const override { return "KeyPress"; }

			static Type getStaticType() { return Type::KeyPress; }
			Type getType() const override { return getStaticType(); }

		private:

			int m_repeatCount;

		};


		export class  KeyRelease : public Key
		{
		public:

			KeyRelease(::bm::Input::Key key) :
				Key(key)
			{}

			std::string toString() const override;
			const char* getName() const override { return "KeyRelease"; }

			static Type getStaticType() { return Type::KeyRelease; }
			Type getType() const override { return getStaticType(); }

		private:

			int m_repeat_count;

		};
	}
	
	//from glfw3.h
	export enum class Input::Key
	{
		SPACE = 32,
		APOSTROPHE = 39,
		COMMA = 44,
		MINUS = 45,
		PERIOD = 46,
		SLASH = 47,
		_0 = 48,
		_1 = 49,
		_2 = 50,
		_3 = 51,
		_4 = 52,
		_5 = 53,
		_6 = 54,
		_7 = 55,
		_8 = 56,
		_9 = 57,
		SEMICOLON = 59,
		EQUAL = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91,
		BACKSLASH = 92,
		RIGHT_BRACKET = 93,
		GRAVE_ACCENT = 96,
		WORLD_1 = 161,
		WORLD_2 = 162,
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		KP_0 = 320,
		KP_1 = 321,
		KP_2 = 322,
		KP_3 = 323,
		KP_4 = 324,
		KP_5 = 325,
		KP_6 = 326,
		KP_7 = 327,
		KP_8 = 328,
		KP_9 = 329,
		KP_DECIMAL = 330,
		KP_DIVIDE = 331,
		KP_MULTIPLY = 332,
		KP_SUBTRACT = 333,
		KP_ADD = 334,
		KP_ENTER = 335,
		KP_EQUAL = 336,
		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		MENU = 348,
	};
	
	export enum class Input::Mouse
	{
		_1 = 0,
		_2 = 1,
		_3 = 2,
		_4 = 3,
		_5 = 4,
		_6 = 5,
		_7 = 6,
		_8 = 7,
		LAST = 7,
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2
	};

}