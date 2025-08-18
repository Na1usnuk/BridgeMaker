#include "pch.hpp"

#include "opengl/opengl_renderer.hpp"
#include "opengl/opengl.hpp"
#include "opengl/opengl_context.hpp"
#include "context.hpp"
#include "platform/window.hpp"
#include "platform/xplatform/xwindow.hpp"


BM_START
GL_START

OpenGLRenderer::OpenGLRenderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}


void OpenGLRenderer::setView(std::array<int, 4> viewport)
{
	auto& current_viewport = m_state_cache[Context::getContext().getCurrent()].viewport;

	if(viewport != current_viewport)
	{
		GLCALL(glViewport(viewport[0], viewport[1], viewport[2], viewport[3]));
		current_viewport = viewport;
	}
}

void OpenGLRenderer::setBackgroundColor(RGBA_t rgba)
{
	auto& current_color = m_state_cache[Context::getContext().getCurrent()].clear_color;

	if (rgba != current_color)
	{
		GLCALL(glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]));
		current_color = rgba;
	}
}

void OpenGLRenderer::setBackgroundColor(RGB_t rgb)
{
	setBackgroundColor({ rgb[0], rgb[1], rgb[2], 1.f });
}

void OpenGLRenderer::clear()
{
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGLRenderer::clearColor(RGBA_t rgba)
{
	setBackgroundColor(rgba);
	clear();
}

void OpenGLRenderer::clearColor(RGB_t rgb)
{
	clearColor({ rgb[0], rgb[1], rgb[2], 1.f });
}

void OpenGLRenderer::setPolygonMode(PolygonMode mode)
{
	auto& current_mode = m_state_cache[Context::getContext().getCurrent()].polygon_mode;

	if(current_mode != mode)
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, static_cast<unsigned int>(mode)));
		current_mode = mode;
	}
}

void OpenGLRenderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh) 
{
	sh.bind();
	va.bind();
	ib.bind();

	GLCALL(glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, nullptr));
}

GL_END
BM_END
