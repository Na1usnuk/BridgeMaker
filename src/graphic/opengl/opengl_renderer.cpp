#include "pch.hpp"

#include "opengl/opengl_renderer.hpp"
#include "opengl/opengl.hpp"


BM_START
GL_START

OpenGLRenderer::OpenGLRenderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}


void OpenGLRenderer::setView(int x, int y, int w, int h) const
{
	GLCALL(glViewport(x, y, w, h));
}

void OpenGLRenderer::clear() const
{
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGLRenderer::setBackgroundColor(float R, float G, float B, float A)
{
	if (m_background_color != std::array<float, 4>{R, G, B, A})
	{
		GLCALL(glClearColor(R, G, B, A));
		m_background_color = std::array<float, 4>{ R, G, B, A };
	}
	BM_DEBUG_CODE(
		float r[4];
		GLCALL(glGetFloatv(GL_COLOR_CLEAR_VALUE, r));
		BM_CORE_TRACE("Actual clear color: {0}, {1}, {2}, {3}", r[0], r[1], r[2], r[3]);
		BM_CORE_TRACE("Cached clear color: {0}, {1}, {2}, {3}", m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
	);
}

void OpenGLRenderer::clearColor(float R, float G, float B, float A)
{
	GLCALL(glClearColor(R, G, B, A));
	clear();
}

void OpenGLRenderer::setPolygonMode(PolygonMode mode) const
{
	GLCALL(glPolygonMode(GL_FRONT_AND_BACK, (unsigned int)mode));
}

void OpenGLRenderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh) const
{
	sh.bind();
	va.bind();
	ib.bind();

	GLCALL(glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, nullptr));
}

void OpenGLRenderer::draw(const Obj& object) const
{
	draw(object.vao, object.ebo, object.shader); 
}

GL_END
BM_END
