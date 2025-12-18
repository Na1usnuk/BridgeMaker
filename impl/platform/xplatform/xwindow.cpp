module;

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "gl_call.hpp"

module bm.platform:window;
import :window;
import :input;
import :cursor;

import bm.core;

namespace bm 
{

	static bool s_isGLFWInitialized = false;
	
	
	Window::Window(std::string_view v, int w, int h, bool vs, bool d, bool vis, Window* shared)
		: m_data( v, w, h, vs, d, vis )
	{
		create(shared);
	}
	
	Window::~Window()
	{
		if(m_window != nullptr)
			destroy();
	}
	
	Window::Window(Window&& oth) noexcept
		: m_data(std::move(oth.m_data)),
		m_window(std::exchange(oth.m_window, nullptr))
	{
	}

	Window& Window::operator=(Window&& oth) noexcept
	{
		if (this != &oth)
		{
			if (m_window != nullptr)
				destroy();
			m_window = std::exchange(oth.m_window, nullptr);
			m_data = std::move(oth.m_data);
		}
		return *this;
	}
	
	void Window::destroy()
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
		Cursor::destroy();
		core::log::trace("Window \"{0}\" is destroyed", m_data.title);
	}
	
	void onFocus(Window::NativeWindow window, int focused)
	{
		if (focused)
		{
			Window::Data* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));
			Input::setCurrentWindow(data->window);
		}
	}
	
	void Window::onUpdate()
	{
		glfwPollEvents();
	}
	
	std::pair<int, int> Window::getPosition() const
	{
		int x, y;
		glfwGetWindowPos(m_window, &x, &y);
		return std::pair<int, int>(x, y);
	}
	
	std::pair<int, int> Window::getFramebufferSize() const
	{
		int w, h;
		glfwGetFramebufferSize(m_window, &w, &h);
		return std::pair<int, int>(w, h);
	}
	
	std::pair<int, int> Window::getSize() const
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		return std::pair<int, int>(w, h);
	}
	
	std::pair<int, int> Window::getFramebufferPosition() const
	{
		auto [x, y] = getPosition();
		int top, left;
		glfwGetWindowFrameSize(m_window, &left, &top, nullptr, nullptr);
		x += left;
		y += top;
		return std::pair<int, int>(x, y);
	}
	
	void Window::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	
		m_data.vsync = enabled;
	}
	
	void Window::setTitle(std::string_view title)
	{
		glfwSetWindowTitle(m_window, title.data());
		m_data.title = title;
	}
	
	void Window::setIcon(Icon icon)
	{
		glfwSetWindowIcon(m_window, 1, (GLFWimage*)&icon);
	}
	
	void Window::resize(int width, int height)
	{
		m_data.height = height;
		m_data.width = width;
	}
	
	void Window::hide()
	{
		glfwHideWindow(m_window);
	}
	
	void Window::show()
	{
		glfwShowWindow(m_window);
	}
	
	void Window::setSize(int width, int height)
	{
		resize(width, height);
		glfwSetWindowSize(m_window, width, height);
	}
	
	void Window::setOpacity(float opacity)
	{
		glfwSetWindowOpacity(m_window, opacity);
	}
	
	void Window::setPosition(int x, int y)
	{
		glfwSetWindowPos(m_window, x, y);
	}
	
	void Window::setCaptureCursor(bool value)
	{
		if(value)
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else 
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
	void Window::close()
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		m_window = nullptr;
		destroy();
		core::log::trace("Window \"{0}\" is closed", m_data.title);
	}
	
	bool Window::isOpen() const
	{
		return not glfwWindowShouldClose(m_window) and (m_window != nullptr);
	}
	
	void Window::create(Window* shared)
	{
		core::log::trace("Creating Window \"{0}\" {1}x{2}", m_data.title, m_data.width, m_data.height);
	
		if (!s_isGLFWInitialized)
		{
			auto success = glfwInit();
			core::verify(success, "Failed to initialize GLFW");
			core::log::info("GLFW initialized");
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			s_isGLFWInitialized = true;
		}
	
		glfwWindowHint(GLFW_DECORATED, m_data.decorated ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, m_data.visible ? GLFW_TRUE : GLFW_FALSE);
	
	
		constexpr auto find_version_index = []() constexpr
			{
				for (std::size_t i = 0; i < gfx::versions.size(); ++i)
					if (::bm::gfx::versions[i] == config::gfx::target_version)
						return i;
				return gfx::versions.size();
			};
	
		constexpr int version_index = find_version_index();
		static_assert(version_index != gfx::versions.size(), "Wrong OpenGL target_version specified");

		GLFWwindow* shared_context = nullptr;
		if (shared != nullptr)
			shared_context = shared->getNative();
	
		for (int i = version_index; i < gfx::versions.size(); ++i)
		{
			auto [major, minor] = gfx::versions[i];
	
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	
			m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, shared_context);
	
			if (m_window != nullptr)
			{
				core::log::trace("Successfully created window {} with OpenGL version {}.{}", m_data.title, major, minor);
				break;
			}
			core::log::warning("Unable to create window with context version {}.{}", major, minor);
		}
		core::verify(m_window, "Failed to create window");
	
		m_data.window = this;
	
		Cursor::init();
		setVSync(m_data.vsync);
		setGLFWPointer();
		setAllCallbacks();
		onFocus(m_window, GLFW_TRUE);
	}
	
	void Window::setGLFWPointer()
	{
		glfwSetWindowUserPointer(m_window, &m_data);
	}
	
	void Window::setKeyCallback()
	{
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				auto k = static_cast<Input::Key>(key);
	
				switch (action)
				{
				case GLFW_PRESS:
				{
					event::KeyPress e(k, 0);
					data->callback(e);
					data->last_key.key = key;
					data->last_key.repeat_count = 0;
					return;
				}
				case GLFW_RELEASE:
				{
					event::KeyRelease e(k);
					data->callback(e);
					if (key == data->last_key.key)
						data->last_key.repeat_count = 0;
					return;
				}
				case GLFW_REPEAT:
				{
					if (key == data->last_key.key)
					{
						data->last_key.repeat_count++;
						event::KeyPress e(k, data->last_key.repeat_count);
						data->callback(e);
					}
					return;
				}
				}
			});
	}
	
	void Window::setMouseButtonCallback()
	{
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				auto b = static_cast<Input::Mouse>(button);

				switch (action)
				{
				case GLFW_PRESS:
				{
					event::MouseButtonPress e(b);
					data->callback(e);
					return;
				}
				case GLFW_RELEASE:
				{
					event::MouseButtonRelease e(b);
					data->callback(e);
					return;
				}
				}
			});
	}
	
	void Window::setResizeCallback()
	{
		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				event::WindowResize e(width, height);
				data->window->resize(width, height);
				data->callback(e);
			});
	}
	
	void Window::setMouseScrollCallback()
	{
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double offset_x, double offset_y) 
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				event::MouseScroll e(offset_x, offset_y);
				data->callback(e);
			});
	}
	
	
	void Window::setAllCallbacks()
	{
		setKeyCallback();
		setMouseButtonCallback();
		setCloseCallback();
		setResizeCallback();
		setPosCallback();
		setMouseMoveCallback();
		setWindowFocusCallback();
		setMouseScrollCallback();
	}
	
	void Window::setMouseMoveCallback()
	{
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				event::MouseMove e(x, y);
				Input::setMousePosition({ (float)x, (float)y });
				data->callback(e);
			});
	}
	
	void Window::setWindowFocusCallback()
	{
		glfwSetWindowFocusCallback(m_window, &onFocus);
	}
	
	void Window::setPosCallback()
	{
		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int x, int y)
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				event::WindowMove e(x, y);
				data->callback(e);
			});
	}
	
	void Window::setCloseCallback()
	{
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				event::WindowClose e(data->window);
				data->callback(e);
			});
	}

	namespace event
	{
		std::string WindowMove::toString() const
		{
			std::stringstream ss;
			ss << "WindowMove: " << m_x << ", " << m_y;
			return ss.str();
		}

		std::string WindowResize::toString() const
		{
			std::stringstream ss;
			ss << "WindowResize: " << m_width << ", " << m_height;
			return ss.str();
		}
	}

}