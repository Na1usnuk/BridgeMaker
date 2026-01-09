module;

#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
	}
	
}