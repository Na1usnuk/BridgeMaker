export module bm.window.manager;

import bm.window;
import bm.config;
import bm.assert;

import std;

namespace bm
{

export class MultiWindowManager;
export class SingleWindowManager;

template<typename WindowManager>
struct WindowTraits
{
};

template<>
struct WindowTraits<MultiWindowManager>
{
	using WindowPtr = std::unique_ptr<Window>;
	using WindowConstPtr = std::unique_ptr<const Window>;
	using WindowRef = Window&;
	using NativeWindowPtr = Window::NativeWindowPtr;
	using WindowContainer = std::vector<WindowPtr>;
	using WindowIterator = WindowContainer::iterator;
	using WindowConstIterator = WindowContainer::const_iterator;
};

template<>
struct WindowTraits<SingleWindowManager>
{
	using WindowPtr = std::unique_ptr<Window>;
	using WindowConstPtr = std::unique_ptr<const Window>;
	using WindowRef = Window&;
	using NativeWindowPtr = Window::NativeWindowPtr;
	using WindowContainer = std::array<WindowPtr, 1>;
	using WindowIterator = WindowContainer::iterator;
	using WindowConstIterator = WindowContainer::const_iterator;
};


export class MultiWindowManager
{

	using Traits = WindowTraits<MultiWindowManager>;

public:

	MultiWindowManager() { m_current_window = m_windows.end(); }

	Traits::WindowRef getCurrent() const
	{
		core::verify(m_current_window != m_windows.end(), "Current Window is nullptr!");

		return **m_current_window;
	}

	void setCurrent(Traits::WindowPtr window)
	{
		auto it = std::find(m_windows.begin(), m_windows.end(), window);

		//verify that window is created and managed
		core::verify(it != m_windows.end(),
			"Window " + window->getTitle() + " are not exist! Current does not change!");

		m_current_window = it;
	}

	void openWindow(std::string_view title = "Bridge Maker App", int width = 1280, int height = 720, bool vsync = false, bool decorated = true, bool visible = true)
	{
		m_windows.emplace_back(std::make_unique<Window>(title, width, height, vsync, decorated, visible));
		if (m_current_window == m_windows.end()) m_current_window = m_windows.begin();
	}

	void closeWindow(Traits::WindowRef window)
	{
		auto it = std::find_if(m_windows.begin(), m_windows.end(), [&window](const Traits::WindowPtr& win) { return &window == win.get(); });
		core::verify(it != m_windows.end(), "Window that is about to be deleted does not exist!");
		m_windows.erase(it);
	}

	Traits::WindowIterator begin() { return m_windows.begin(); }
	Traits::WindowIterator end() { return m_windows.end(); }
	Traits::WindowConstIterator begin() const { return cbegin(); }
	Traits::WindowConstIterator end() const { return cend(); }
	Traits::WindowConstIterator cbegin() const { return m_windows.cbegin(); }
	Traits::WindowConstIterator cend() const { return m_windows.cend(); }


private:

	Traits::WindowContainer m_windows;
	Traits::WindowIterator m_current_window;

};


export class SingleWindowManager
{

	using Traits = WindowTraits<SingleWindowManager>;

public:

	SingleWindowManager() : m_window({ nullptr }) {}

	//close current window and create new one
	void openWindow(std::string_view title = "Bridge Maker App", int width = 1280, int height = 720, bool vsync = false, bool decorated = true, bool visible = true)
	{
		Traits::WindowPtr tmp = std::make_unique<Window>(title, width, height, vsync, decorated, visible);
		m_window.front().swap(tmp);
	}

	Traits::WindowRef getCurrent() const
	{ 
		core::verify(not m_window.empty(), "Current Window is nullptr!");
		return *m_window.front(); 
	}

	void closeWindow()
	{

	}

	Traits::WindowIterator begin() { return m_window.begin(); }
	Traits::WindowIterator end() { return m_window.end(); }
	Traits::WindowConstIterator begin() const { return cbegin(); }
	Traits::WindowConstIterator end() const { return cend(); }
	Traits::WindowConstIterator cbegin() const { return m_window.cbegin(); }
	Traits::WindowConstIterator cend() const { return m_window.cend(); }


private:

	Traits::WindowContainer m_window;

};

}