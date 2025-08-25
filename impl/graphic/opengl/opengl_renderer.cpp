module;

#include "glad/glad.h"

module bm.gfx.renderer;

import bm.gfx.utility;
import bm.gfx.buffer.index;
import bm.gfx.buffer.vertex;
import bm.gfx.vertexarray;
import bm.gfx.shader;
import bm.gfx.context;

import bm.window;


import std;


namespace bm::gfx
{
	
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}


void Renderer::setView(std::array<int, 4> viewport)
{
	auto& current_viewport = m_state_cache[Context::getContext().getCurrent()].viewport;

	if(viewport != current_viewport)
	{
		glCall(glViewport, viewport[0], viewport[1], viewport[2], viewport[3]);
		current_viewport = viewport;
	}
}

void Renderer::setBackgroundColor(RGBA_t rgba)
{
	auto& current_color = m_state_cache[Context::getContext().getCurrent()].clear_color;

	if (rgba != current_color)
	{
		glCall(glClearColor, rgba[0], rgba[1], rgba[2], rgba[3]);
		current_color = rgba;
	}
}

void Renderer::setBackgroundColor(RGB_t rgb)
{
	setBackgroundColor({ rgb[0], rgb[1], rgb[2], 1.f });
}

void Renderer::clear()
{
	glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clearColor(RGBA_t rgba)
{
	setBackgroundColor(rgba);
	clear();
}

void Renderer::clearColor(RGB_t rgb)
{
	clearColor({ rgb[0], rgb[1], rgb[2], 1.f });
}

void Renderer::setPolygonMode(PolygonMode mode)
{
	auto& current_mode = m_state_cache[Context::getContext().getCurrent()].polygon_mode;

	if(current_mode != mode)
	{
		glCall(glPolygonMode, GL_FRONT_AND_BACK, static_cast<unsigned int>(mode));
		current_mode = mode;
	}
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh)
{
	sh.bind();
	va.bind();
	ib.bind();

	glCall(glDrawElements, GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, nullptr);
}

}

