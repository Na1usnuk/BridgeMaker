#include "pch.hpp"
#include "core/log.hpp"

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
	init(data);
}

WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(m_window);
}

void WindowsWindow::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void WindowsWindow::setVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_data.vsync = enabled;
}

void WindowsWindow::init(const Data& data)
{
	m_data = data;

	BM_CORE_INFO("Creating Windows Window \"{0}\" {1}x{2}", m_data.title, m_data.width, m_data.height);

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
	if (!m_window)
	{
		BM_CORE_ERROR("Failed to create window");
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);
	setVSync(m_data.vsync);

	BM_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize Glad");


}

void WindowsWindow::close()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

bool WindowsWindow::isOpen() const
{
	return !glfwWindowShouldClose(m_window);
}

BM_END