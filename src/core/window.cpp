#include "pch.hpp"
#include "core/log.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"
#include "window.hpp"
#include "opengl/opengl_context.hpp"

#include "GLFW/glfw3.h"

BM_START

static bool s_isGLFWInitialized = false;


Window::Window(const Data& data)
{
	BM_TIME_OF("Windows Window initialization time", create(data));
}

Window::~Window()
{
	destroy();
}

void Window::destroy()
{
	glfwDestroyWindow(m_window);
	m_window = nullptr;
	BM_CORE_TRACE("Window \"{0}\" is destroyed", m_data.title);
}

void Window::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

std::pair<int, int> Window::getPosition() const
{
	int x, y;
	glfwGetWindowPos(m_window, &x, &y);
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

void Window::setIcon(Icon path)
{
	glfwSetWindowIcon(m_window, 1, (GLFWimage*)&path);
}

void Window::resize(int width, int height)
{
	m_data.height = height;
	m_data.width = width;
	BM_TRACE("Window \"{0}\" is resized to {1}x{2}", m_data.title, m_data.width, m_data.height);
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

void Window::close()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	m_window = nullptr;
	BM_CORE_TRACE("Window \"{0}\" is closed", m_data.title);
}

bool Window::isOpen() const
{
	return !glfwWindowShouldClose(m_window) && (m_window != nullptr);
}

void Window::create(const Data& data)
{
	m_data = data;
	m_data.window = this;

	BM_CORE_INFO("Creating Window \"{0}\" {1}x{2}", m_data.title, m_data.width, m_data.height);

	if (!s_isGLFWInitialized)
	{
		auto success = glfwInit();
		BM_CORE_ASSERT(success, "Failed to initialize GLFW");
		BM_CORE_INFO("GLFW initialized");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		s_isGLFWInitialized = true;
	}

	GL::OpenGLContext& ctx = GL::OpenGLContext::getContext();

	m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, ctx.shareContext());
	BM_CORE_ASSERT(m_window, "Failed to create window");

	ctx.makeCurrent(this);
	ctx.init();
	setVSync(m_data.vsync);
	setGLFWPointer();
	setAllCallbacks();
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

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent e(key, 0);
				data->callback(e);
				data->last_key.key = key;
				data->last_key.repeat_count = 0;
				return;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent e(key);
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
					KeyPressedEvent e(key, data->last_key.repeat_count);
					data->callback(e);
				}
				return;
			}
			}
		});
}

void Window::setMuoseButtonCallback()
{
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent e(button);
				data->callback(e);
				return;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent e(button);
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
			WindowResizeEvent e(width, height);
			e.setWindow(data->window);
			data->callback(e);
		});
	
}

void Window::setAllCallbacks()
{
	setKeyCallback();
	setMuoseButtonCallback();
	setCloseCallback();
	setResizeCallback();
	setPosCallback();
	setMouseMoveCallback();
}

void Window::setMouseMoveCallback()
{
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			MouseMoveEvent e(x, y);
			e.setWindow(data->window);
			data->callback(e);
		});
}

void Window::setPosCallback()
{
	glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int x, int y)
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			WindowMoveEvent e(x, y);
			e.setWindow(data->window);
			data->callback(e);
		});
}

void Window::setCloseCallback()
{
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			e.setWindow(data->window);
			data->callback(e);
		});
}

BM_END