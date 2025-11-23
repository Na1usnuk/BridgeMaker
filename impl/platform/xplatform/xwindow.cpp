module;

#include "glad/glad.h"
#include "GLFW/glfw3.h"

module bm.window;

import bm.log;
import bm.cursor;
import bm.input;
import bm.verify;
import bm.event;
import bm.gfx.utility;

namespace bm 
{

static bool s_isGLFWInitialized = false;


Window::Window(std::string_view v, int w, int h, bool vs, bool decorated, bool visible)
	: m_data( v, w, h, vs )
{
	create(decorated, visible);
}

Window::~Window()
{
	destroy();
}


void Window::destroy()
{
	glfwDestroyWindow(m_window);
	m_window = nullptr;
	Cursor::destroy();
	log::core::trace("Window \"{0}\" is destroyed", m_data.title);
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
	log::core::trace("Window \"{0}\" is closed", m_data.title);
}

bool Window::isOpen() const
{
	return !glfwWindowShouldClose(m_window) && (m_window != nullptr);
}

void Window::create(bool decorated, bool visible)
{
	log::core::trace("Creating Window \"{0}\" {1}x{2}", m_data.title, m_data.width, m_data.height);

	if (!s_isGLFWInitialized)
	{
		auto success = glfwInit();
		core::verify(success, "Failed to initialize GLFW");
		log::core::info("GLFW initialized");
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		s_isGLFWInitialized = true;
	}

	glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);

	constexpr std::array<std::pair<int, int>, 8> versions =
	{
		std::pair<int, int>{4, 6},
		std::pair<int, int>{4, 5},
		std::pair<int, int>{4, 4},
		std::pair<int, int>{4, 3},
		std::pair<int, int>{4, 2},
		std::pair<int, int>{4, 1},
		std::pair<int, int>{4, 0},
		std::pair<int, int>{3, 3}
	};

	for (auto[major, minor] : versions)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, gfx::Context::get().shareContext());

		if (m_window != nullptr)
		{
			log::core::trace("Successfully created window {} with OpenGL version {}.{}", m_data.title, major, minor);
			gfx::Context::get().setVersion(major * 10 + minor);
			break;
		}
	}
	core::verify(m_window, "Failed to create window");

	m_data.window = this;

	gfx::Context::get().setCurrent(*this);
	gfx::Context::get().init();
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

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressEvent e(key, 0);
				data->callback(e);
				data->last_key.key = key;
				data->last_key.repeat_count = 0;
				return;
			}
			case GLFW_RELEASE:
			{
				KeyReleaseEvent e(key);
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
					KeyPressEvent e(key, data->last_key.repeat_count);
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
				MouseButtonPressEvent e(button);
				data->callback(e);
				return;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent e(button);
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
			data->window->resize(width, height);
			data->callback(e);
		});
}

void Window::setMouseScrollCallback()
{
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double offset_x, double offset_y) 
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			MouseScrollEvent e(offset_x, offset_y);
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
	setWindowFocusCallback();
	setMouseScrollCallback();
}

void Window::setMouseMoveCallback()
{
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			MouseMoveEvent e(x, y);
			Input::setMousePos({ (float)x, (float)y });
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
			WindowMoveEvent e(x, y);
			data->callback(e);
		});
}

void Window::setCloseCallback()
{
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			data->callback(e);
		});
}


namespace gfx
{

	Context Context::s_ctx_inst{};


	Context& Context::get()
	{
		return s_ctx_inst;
	}

	//initialize Glad context
	void Context::init()
	{
		auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		core::verify(status, "Failed to initialize Glad");
		log::core::info("Glad initialized");

		const char* renderer = reinterpret_cast<const char*>(glCall(glGetString, GL_RENDERER));
		const char* vendor = reinterpret_cast<const char*>(glCall(glGetString, GL_VENDOR));
		const char* version = reinterpret_cast<const char*>(glCall(glGetString, GL_VERSION));
		const char* glsl_version = reinterpret_cast<const char*>(glCall(glGetString, GL_SHADING_LANGUAGE_VERSION));

		log::core::info("Renderer: {}", renderer);
		log::core::info("Vendor: {}", vendor);
		log::core::info("OpenGL version: {}", version);
		log::core::info("GLSL version: {}", glsl_version);

	}

	void Context::setCurrent(Window& window)
	{
		m_window = &window;
		glfwMakeContextCurrent(m_window->getNativeWindow());
	}

	Window& Context::getCurrent() const
	{
		core::verify(m_window != nullptr, "Current context is invalid");
		return *m_window;
	}

	void Context::destroy()
	{
		glfwTerminate();
	}

	void Context::swapBuffers()
	{
		core::verify(m_window != nullptr, "Current context is invalid");
		glfwSwapBuffers(m_window->getNativeWindow());
	}

	//share context with other windows
	Window::NativeWindow Context::shareContext()
	{
		if (m_window != nullptr)
			return m_window->getNativeWindow();
		return nullptr;
	}

}

}