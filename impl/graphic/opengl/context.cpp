module bm.gfx:context;
import :context;
import :utility;

import bm.platform;

import <glad/glad.h>;
import <GLFW/glfw3.h>;
import <gl_call.hpp>;

namespace bm::gfx
{

	Context* Context::s_current_ctx = nullptr;

	Context::Context(bm::Window& window)
		: m_window(&window)
	{
		makeCurrent();
		init();
	}

	Context::Context(bm::Window& window, Context& shared)
		: m_window(&window)
	{
		makeCurrent();
		init(window, shared);
	}

	void Context::makeCurrent()
	{
		s_current_ctx = this;
		glfwMakeContextCurrent(m_window->getNative());
	}

	void Context::init(bm::Window& window, Context& shared)
	{
		core::log::trace("Initializing shared context");

		window = Window(
			window.getTitle(), 
			window.getWidth(), 
			window.getHeight(), 
			window.getVSync(), 
			window.isDecorated(), 
			window.isVisible(), 
			&shared.getWindow());

		infoPrint();
		recognizeVersion();
	}

	//initialize Glad context
	void Context::init()
	{
		core::log::trace("Initializing context for {} window", m_window->getTitle());
		auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		core::verify(status, "Failed to initialize Glad");
		core::log::info("Glad initialized");

		infoPrint();
		recognizeVersion();
	}

	void Context::recognizeVersion()
	{
		int major, minor;

		glCall(glGetIntegerv, GL_MAJOR_VERSION, &major);
		glCall(glGetIntegerv, GL_MINOR_VERSION, &minor);

		m_gl_version = { major, minor };
	}

	void Context::infoPrint()
	{
		const char* renderer = reinterpret_cast<const char*>(glCall(glGetString, GL_RENDERER));
		const char* vendor = reinterpret_cast<const char*>(glCall(glGetString, GL_VENDOR));
		const char* version = reinterpret_cast<const char*>(glCall(glGetString, GL_VERSION));
		const char* glsl_version = reinterpret_cast<const char*>(glCall(glGetString, GL_SHADING_LANGUAGE_VERSION));

		core::log::info("Renderer: {}", renderer);
		core::log::info("Vendor: {}", vendor);
		core::log::info("OpenGL version: {}", version);
		core::log::info("GLSL version: {}", glsl_version);
	}

	bm::Window& Context::getWindow() const
	{
		core::verify(m_window, "Current context is invalid");
		return *m_window;
	}

	void Context::destroy()
	{
		glfwTerminate();
	}

	void Context::swapBuffers()
	{
		core::verify(m_window, "Current context is invalid");
		glfwSwapBuffers(m_window->getNative());
	}

}