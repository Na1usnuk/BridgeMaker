#pragma once

#include "core/core.hpp"

#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"

#include <unordered_map>


BM_START
GL_START

class OpenGLRenderer
{
public:

	enum class PolygonMode : unsigned int
	{
		//magic numbers from GL
		Fill = 0x1B02,
		Line = 0x1B01,
		Point = 0x1B00
	};

	using VertexArray  = ::BM::GL::VertexArray;
	using VertexBuffer = ::BM::GL::VertexBuffer;
	using IndexBuffer  = ::BM::GL::IndexBuffer;
	using Shader       = ::BM::GL::Shader;

	using RGB_t      = std::array<float, 3>;
	using RGBA_t     = std::array<float, 4>;

private:

	struct StateCache
	{
		RGBA_t clear_color;
		PolygonMode polygon_mode;
		std::array<int, 4> viewport;
	};

public:

	OpenGLRenderer();
	~OpenGLRenderer();

	void clear();
	void draw(const VertexArray&, const IndexBuffer&, const Shader&);
	
	void setPolygonMode(PolygonMode mode = PolygonMode::Fill);
	void setView(std::array<int, 4> viewport);
	void setBackgroundColor(RGBA_t rgba = {0.f, 0.f, 0.f, 1.f});
	void setBackgroundColor(RGB_t rgb = {0.f, 0.f, 0.f});
	void clearColor(RGBA_t rgba = { 0.f, 0.f, 0.f, 1.f });
	void clearColor(RGB_t rgb = { 0.f, 0.f, 0.f });


private:

	std::unordered_map<Window*, StateCache> m_state_cache;

};

GL_END
BM_END