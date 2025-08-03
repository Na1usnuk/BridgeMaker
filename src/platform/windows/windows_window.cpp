#include "pch.hpp"
#include "core/log.hpp"
#include "events/app_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"


#include "platform/windows/windows_window.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

BM_START

static bool s_isGLFWInitialized = false;

std::unique_ptr<AbstractWindow> AbstractWindow::create(const Data& props)
{
	return std::make_unique<WindowsWindow>(props);
}


WindowsWindow::WindowsWindow(const AbstractWindow::Data& data)
{
	BM_TIMING_OF("Windows Window initialization time", init(data));
}

WindowsWindow::~WindowsWindow()
{
	BM_CORE_TRACE("Window \"{0}\" is destroyed", m_data.title);
	glfwDestroyWindow(m_window);
}

void WindowsWindow::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}


inline void WindowsWindow::setVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_data.vsync = enabled;
}

void WindowsWindow::resize(int width, int height)
{
	m_data.height = height;
	m_data.width = width;
	BM_TRACE("Window \"{0}\" is resized to {1}x{2}", m_data.title, m_data.width, m_data.height);
}

void WindowsWindow::init(const Data& data)
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
			BM_CORE_TRACE("Window close event occured");
			AbstractWindow::Data* data = static_cast<AbstractWindow::Data*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			data->callback(e);
		});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) 
		{
			BM_CORE_TRACE("Window resize event occured: {0}x{1}", width, height);
			AbstractWindow::Data* data = static_cast<AbstractWindow::Data*>(glfwGetWindowUserPointer(window));
			WindowResizeEvent e(width, height);
			data->callback(e);
		});

}

inline void WindowsWindow::close()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	BM_CORE_TRACE("Window \"{0}\" is closed", m_data.title);
}

inline bool WindowsWindow::isOpen() const
{
	return !glfwWindowShouldClose(m_window);
}

BM_END