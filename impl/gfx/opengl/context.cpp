module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GL_VERIFY(func, ...) glVerify( std::source_location::current(), func, __VA_ARGS__)

module bm.gfx:context;

import std;

import :context;
import :verify;

import bm.platform;

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

	void Context::makeCurrent() noexcept
	{
		s_current_ctx = this;
		glfwMakeContextCurrent(m_window->getNative());
	}

	void Context::setVSync(bool enabled) noexcept
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_vsync = enabled;
	}

	void Context::init(bm::Window& window, Context& shared)
	{
		core::log::trace("Initializing shared context");

		window = Window(
			window.getTitle(), 
			window.getWidth(), 
			window.getHeight(), 
			window.isDecorated(), 
			window.isVisible(), 
			&shared.getWindow());

		setVSync(m_vsync);

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

		setVSync(m_vsync);

		infoPrint();
		recognizeVersion();
	}

	void Context::recognizeVersion()
	{
		int major, minor;

		GL_VERIFY(glGetIntegerv, GL_MAJOR_VERSION, &major);
		GL_VERIFY(glGetIntegerv, GL_MINOR_VERSION, &minor);

		m_gl_version = { major, minor };
	}

	void Context::infoPrint()
	{
		const char* renderer = reinterpret_cast<const char*>(GL_VERIFY(glGetString, GL_RENDERER));
		const char* vendor = reinterpret_cast<const char*>(GL_VERIFY(glGetString, GL_VENDOR));
		const char* version = reinterpret_cast<const char*>(GL_VERIFY(glGetString, GL_VERSION));
		const char* glsl_version = reinterpret_cast<const char*>(GL_VERIFY(glGetString, GL_SHADING_LANGUAGE_VERSION));

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

	void Context::swapBuffers() noexcept
	{
		core::verify(m_window, "Current context is invalid");
		glfwSwapBuffers(m_window->getNative());
	}

}