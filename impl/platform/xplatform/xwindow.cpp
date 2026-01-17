module;

#include "glad/glad.h"
#include "GLFW/glfw3.h"

module bm.platform:window;
import :window;
import :input;

import bm.config;
import bm.core;

namespace bm::platform
{

	static bool s_isGLFWInitialized = false;
	
	
	Window::Window(std::string_view title, int width, int height, bool decorated, bool visible, Window* shared) noexcept
	{
		core::log::trace("Creating Window \"{0}\" {1}x{2}", title, width, height);

		if (!s_isGLFWInitialized)
		{
			auto success = glfwInit();
			core::verify(success, "Failed to initialize GLFW");
			core::log::info("GLFW initialized");
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			s_isGLFWInitialized = true;
		}

		glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);


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

			m_window = glfwCreateWindow(width, height, title.data(), nullptr, shared_context);

			if (m_window != nullptr)
			{
				core::log::trace("Successfully created window {} with OpenGL version {}.{}", title, major, minor);
				break;
			}
			core::log::warning("Unable to create window with context version {}.{}", major, minor);
		}
		core::verify(m_window, "Failed to create window");
	}
	
	Window::~Window()
	{
		if(m_window != nullptr)
			destroy();
	}
	
	Window::Window(Window&& oth) noexcept :
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
		}
		return *this;
	}
	
	void Window::destroy() noexcept
	{
		core::log::trace("Window \"{0}\" is destroyed", getTitle());
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}
	
	
	std::pair<int, int> Window::getPosition() const noexcept
	{
		int x, y;
		glfwGetWindowPos(m_window, &x, &y);
		return { x, y };
	}
	
	std::pair<int, int> Window::getFramebufferSize() const noexcept
	{
		int w, h;
		glfwGetFramebufferSize(m_window, &w, &h);
		return { w, h };
	}
	
	std::pair<int, int> Window::getSize() const noexcept
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		return { w, h };
	}
	
	std::pair<int, int> Window::getFramebufferPosition() const noexcept
	{
		auto [x, y] = getPosition();
		int top, left;
		glfwGetWindowFrameSize(m_window, &left, &top, nullptr, nullptr);
		x += left;
		y += top;
		return { x, y };
	}

	Window::NativeWindow Window::getNative() const noexcept
	{
		return m_window;
	}

	std::string_view Window::getTitle() const noexcept
	{ 
		return glfwGetWindowTitle(m_window);
	}

	int Window::getWidth() const noexcept
	{ 
		return getSize().first;
	}

	int Window::getHeight() const noexcept
	{ 
		return getSize().second;
	}

	bool Window::isDecorated() const noexcept
	{ 
		return glfwGetWindowAttrib(m_window, GLFW_DECORATED);
	}

	bool Window::isVisible() const noexcept
	{ 
		return glfwGetWindowAttrib(m_window, GLFW_VISIBLE);
	}
	
	void Window::setTitle(std::string_view title) noexcept
	{
		glfwSetWindowTitle(m_window, title.data());
	}
	
	void Window::hide() noexcept
	{
		glfwHideWindow(m_window);
	}
	 
	void Window::show() noexcept
	{
		glfwShowWindow(m_window);
	}
	
	void Window::setSize(int width, int height) noexcept
	{
		glfwSetWindowSize(m_window, width, height);
	}
	
	void Window::setOpacity(float opacity) noexcept
	{
		glfwSetWindowOpacity(m_window, opacity);
	}
	
	void Window::setPosition(int x, int y) noexcept
	{
		glfwSetWindowPos(m_window, x, y);
	}
	
	void Window::setCaptureCursor(bool value) noexcept
	{
		if(value)
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else 
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
	void Window::close() noexcept
	{
		core::log::trace("Window \"{0}\" is closed", getTitle());
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}
	
	bool Window::isOpen() const noexcept
	{
		return m_window not_eq nullptr and not glfwWindowShouldClose(m_window);
	}

}