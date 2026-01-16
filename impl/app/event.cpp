module;

#include <GLFW/glfw3.h>

module bm.app:event;

import :event;

import bm.platform;
import bm.core;

import std;

namespace bm::app
{

	EventSystem::EventSystem(const platform::Window& window) noexcept :
		m_window(window)
	{
		core::verify(glfwGetWindowUserPointer(m_window.getNative()) == nullptr,
			"Cannot create multiple EventSystems for same Window");

		glfwSetWindowUserPointer(m_window.getNative(), &m_callbacks);

		// Window callbacks
		glfwSetWindowSizeCallback(m_window.getNative(),
			[](GLFWwindow* window, int width, int height)
			{
				auto& callbacks = *static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
				callbacks.window.resize(width, height);
			});

		glfwSetWindowPosCallback(m_window.getNative(),
			[](GLFWwindow* window, int x, int y)
			{
				auto& callbacks = *static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
				callbacks.window.move(x, y);
			});

		glfwSetWindowCloseCallback(m_window.getNative(),
			[](GLFWwindow* window)
			{
				auto& callbacks = *static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
				callbacks.window.close();
			});

		// Key callbacks
		glfwSetKeyCallback(m_window.getNative(),
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto& callbacks = *static_cast<Callbacks*>(glfwGetWindowUserPointer(window));

				switch(action)
				{
					case GLFW_PRESS:
					{
						callbacks.key.last = static_cast<platform::Input::Key>(key);
						callbacks.key.repeat = 0;
						callbacks.key.press(callbacks.key.last, 0);
						break;
					}
					case GLFW_RELEASE:
					{
						callbacks.key.release(static_cast<platform::Input::Key>(key));
						callbacks.key.repeat = 0;
						break;
					}
					case GLFW_REPEAT:
					{
						callbacks.key.repeat++;
						callbacks.key.press(callbacks.key.last, callbacks.key.repeat);
						break;
					}
				}
			});

		// Mouse callbacks
		glfwSetCursorPosCallback(m_window.getNative(),
			[](GLFWwindow* window, double x, double y)
			{
				auto& callbacks = *static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
				callbacks.mouse.move(x, y);
			});

		glfwSetMouseButtonCallback(m_window.getNative(),
			[](GLFWwindow* window, int button, int action, int mods)
			{
				auto& callbacks = *static_cast<Callbacks*>(glfwGetWindowUserPointer(window));

				if (action == GLFW_PRESS)
					callbacks.mouse.press(static_cast<platform::Input::Mouse>(button));
				else if (action == GLFW_RELEASE)
					callbacks.mouse.release(static_cast<platform::Input::Mouse>(button));
			});

		glfwSetScrollCallback(m_window.getNative(),
			[](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto& callbacks = *static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
				callbacks.mouse.scroll(xoffset, yoffset);
			});
	}


	EventSystem::~EventSystem()
	{
		glfwSetWindowUserPointer(m_window.getNative(), nullptr);
		glfwSetWindowSizeCallback(m_window.getNative(), nullptr);
		glfwSetWindowPosCallback(m_window.getNative(), nullptr);
		glfwSetWindowCloseCallback(m_window.getNative(), nullptr);
		glfwSetKeyCallback(m_window.getNative(), nullptr);
		glfwSetCursorPosCallback(m_window.getNative(), nullptr);
		glfwSetMouseButtonCallback(m_window.getNative(), nullptr);
		glfwSetScrollCallback(m_window.getNative(), nullptr);
	}

	void EventSystem::pollEvents() const noexcept
	{
		glfwPollEvents();
	}

	EventSystem::Callbacks::Callbacks()
	{
		// Default universal callback
		event = [](Event&) {};

		// Window callbacks
		window.resize = [this](int w, int h) 
			{
				WindowResizeEvent e(w, h);
				event(e);
			};
		window.move = [this](int x, int y) 
			{
				WindowMoveEvent e(x, y);
				event(e);
			};
		window.close = [this]() 
			{
				WindowCloseEvent e;
				event(e);
			};

		// Key callbacks
		key.press = [this](platform::Input::Key k, unsigned int repeat)
			{
				KeyPressEvent e(k, repeat);
				event(e);
			};
		key.release = [this](platform::Input::Key k)
			{
				KeyReleaseEvent e(k);
				event(e);
			};

		// Mouse callbacks
		mouse.move = [this](double x, double y) 
			{
				MouseMoveEvent e(x, y);
				event(e);
			};
		mouse.press = [this](platform::Input::Mouse button)
			{
				MouseButtonPressEvent e(button);
				event(e);
			};
		mouse.release = [this](platform::Input::Mouse button)
			{
				MouseButtonReleaseEvent e(button);
				event(e);
			};
		mouse.scroll = [this](double x, double y) 
			{
				MouseScrollEvent e(x, y);
				event(e);
			};
	}
}