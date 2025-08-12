#pragma once

#include "core/core.hpp"

#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"


BM_START
GL_START

class OpenGLRenderer
{
public:

	enum class PolygonMode : unsigned int
	{
		Fill = 0x1B02,
		Line = 0x1B01,
		Point = 0x1B00
	};
	
	struct Obj
	{
		Obj(VertexArray& va, IndexBuffer& eb, Shader& sh) : vao(std::move(va)), ebo(std::move(eb)), shader(std::move(sh)) {}
		//~Obj() { vao.destroy(); ebo.destroy(); shader.destroy(); }
		void bind() const { vao.bind(); ebo.bind(); shader.bind(); }
		VertexArray vao;
		IndexBuffer ebo;
		Shader shader;
	};

public:

	OpenGLRenderer();
	~OpenGLRenderer();

	void clear() const;
	void draw(const VertexArray&, const IndexBuffer&, const Shader&) const;
	void draw(const Obj& object) const;
	
	void setPolygonMode(PolygonMode mode = PolygonMode::Fill) const;
	void setView(int x, int y, int w, int h) const;
	void setBackgroundColor(float R, float G, float B, float A = 1.f);

	void clearColor(float R, float G, float B, float A);

private:

	std::array<float, 4> m_background_color = {.0f, .0f, .0f, 1.f};

};

GL_END
BM_END