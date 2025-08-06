#include "pch.hpp"
#include "core/log.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"


#include "window.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

BM_START

static bool s_isGLFWInitialized = false;


Window::Window(const Data& data)
{
	BM_TIME_OF("Windows Window initialization time", init(data));
}

Window::~Window()
{
	BM_CORE_TRACE("Window \"{0}\" is destroyed", m_data.title);
	glfwDestroyWindow(m_window);
}

void Window::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void Window::setVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_data.vsync = enabled;
}

void Window::resize(int width, int height)
{
	m_data.height = height;
	m_data.width = width;
	BM_TRACE("Window \"{0}\" is resized to {1}x{2}", m_data.title, m_data.width, m_data.height);
}

void Window::close()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	BM_CORE_TRACE("Window \"{0}\" is closed", m_data.title);
}

bool Window::isOpen() const
{
	return !glfwWindowShouldClose(m_window);
}

void Window::init(const Data& data)
{
	m_data = data;

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

	m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
	BM_CORE_ASSERT(m_window, "Failed to create window");

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);
	setVSync(m_data.vsync);

	BM_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize Glad");
	BM_CORE_INFO("Glad initialized");

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) 
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			data->callback(e);
		});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) 
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			WindowResizeEvent e(width, height);
			data->callback(e);
		});

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

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		Data * data = static_cast<Data*>(glfwGetWindowUserPointer(window));

		static struct { int key, repeat = 0; } last_key;

		switch (action)
			{
			case GLFW_PRESS:
			{
				last_key.key = key;
				KeyPressedEvent e(key, last_key.repeat);
				data->callback(e);
				return;
			}
			case GLFW_RELEASE:
			{
				if (key == last_key.key)
					last_key.repeat = 0;
				KeyReleasedEvent e(key);
				data->callback(e);
				return;
			}
			case GLFW_REPEAT:
			{
				if (key == last_key.key)
					last_key.repeat++;
				return;
			}	
		}
	});
}

BM_END