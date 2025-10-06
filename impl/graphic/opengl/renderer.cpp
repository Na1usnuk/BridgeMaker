module;

#include "glad/glad.h"

module bm.gfx.renderer;

import bm.gfx.utility;
import bm.gfx.buffer.index;
import bm.gfx.buffer.vertex;
import bm.gfx.vertexarray;
import bm.gfx.shader;
import bm.assetmanager;

import bm.window;

import bm.log;

import std;


namespace bm::gfx
{
	
Renderer::Renderer()
{
	//log::core::trace("glGetError is {}", (void*)glGetError);
	//glCall(glGetIntegerv, GL_MAX_TEXTURE_IMAGE_UNITS, &m_state_cache.texture_slot_count);
}

Renderer::~Renderer()
{
}


void Renderer::setView(const std::array<int, 4>& viewport)
{
	auto& current_viewport = m_state_cache.viewport;

	if(viewport != current_viewport)
	{
		glCall(glViewport, viewport[0], viewport[1], viewport[2], viewport[3]);
		current_viewport = viewport;
	}
}

void Renderer::setBackgroundColor(const RGBA_t& rgba)
{
	auto& current_color = m_state_cache.clear_color;

	if (rgba != current_color)
	{
		glCall(glClearColor, rgba[0], rgba[1], rgba[2], rgba[3]);
		current_color = rgba;
	}
}

void Renderer::setDepthTesting(bool value)
{
	if (m_state_cache.depth_test == value)
		return;

	if (value)
		glCall(glEnable, GL_DEPTH_TEST);
	else
		glCall(glDisable, GL_DEPTH_TEST);

	m_state_cache.depth_test = value;
}

void Renderer::setDepthTestFunc(DepthFunc func)
{
	if (m_state_cache.depth_test_func == func)
		return;

	glCall(glDepthFunc, static_cast<int>(func));

	m_state_cache.depth_test_func = func;
}

void Renderer::setBlend(bool value)
{
	if (m_state_cache.blend == value)
		return;

	if (value)
		glCall(glEnable, GL_BLEND);
	else
		glCall(glDisable, GL_BLEND);

	m_state_cache.blend = value;
}

void Renderer::setBlendFunc(BlendSrc src, BlendFunc func)
{
	//if (m_state_cache.blend_func == func)
	//	return;

	glCall(glBlendFunc, static_cast<int>(src), static_cast<int>(func));
}

void Renderer::setBackgroundColor(const RGB_t& rgb)
{
	setBackgroundColor({ rgb[0], rgb[1], rgb[2], 1.f });
}

void Renderer::clear()
{
	glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clearColor(const RGBA_t& rgba)
{
	setBackgroundColor(rgba);
	clear();
}

void Renderer::clearColor(const RGB_t& rgb)
{
	clearColor({ rgb[0], rgb[1], rgb[2], 1.f });
}

void Renderer::setPolygonMode(PolygonMode mode)
{
	auto& current_mode = m_state_cache.polygon_mode;

	if(current_mode != mode)
	{
		glCall(glPolygonMode, GL_FRONT_AND_BACK, static_cast<unsigned int>(mode));
		current_mode = mode;
	}
}

void Renderer::draw(VertexArray::KPtrRef vao, IndexBuffer::KPtrRef ibo, Shader::KPtrRef shader)
{
	shader->bind();
	vao->bind();
	ibo->bind();

	glCall(glDrawElements, static_cast<int>(vao->getDrawAs()), ibo->count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::draw(VertexArray::KPtrRef vao, Shader::KPtrRef shader)
{
	shader->bind();
	vao->bind();

	glCall(glDrawArrays, static_cast<int>(vao->getDrawAs()), 0, vao->getVerticesCount());
}

void Renderer::draw(Traits<Mesh>::KPtrRef mesh, Traits<Material>::KPtrRef material)
{
	if (mesh->getIndexBuffer() != nullptr)
		draw(mesh->getVertexArray(), mesh->getIndexBuffer(), material->getShader());
	else
		draw(mesh->getVertexArray(), material->getShader());
}

void Renderer::draw(Object::KPtrRef obj, Camera::KPtrRef camera)
{
	auto material = obj->getMaterial();
	material->setUniform("u_model", obj->getModel());
	material->setUniform("u_view", camera->getView());
	material->setUniform("u_projection", camera->getProjection());
	material->setUniform("u_color", material->getColor());

	material->getTexture()->bind();
	material->bind();

	draw(obj->getMesh(), material);
}

void Renderer::draw(Scene::KPtrRef scene, Camera::KPtrRef camera)
{
	for (const auto& obj : scene->getObjects())
		draw(obj, camera);
}

}

