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
	glCall(glGetIntegerv, GL_MAX_TEXTURE_IMAGE_UNITS, &m_state_cache.texture_slot_count);
	m_state_cache.bound_textures.resize(m_state_cache.texture_slot_count, -1);
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

void Renderer::draw(Traits<VertexArray>::KPtrRef vao, Traits<Shader>::KSPtrRef shader, Mesh::DrawAs draw_as)
{
	// Shader should be already bound and uniforms set before calling this method
	//shader->bind();
	vao->bind();

	if(vao->getIndexBuffer() != nullptr)
		glCall(glDrawElements, static_cast<int>(draw_as), vao->getIndexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
	else
		glCall(glDrawArrays, static_cast<int>(draw_as), 0, vao->getVerticesCount());
}


void Renderer::draw(Traits<Scene>::PtrRef scene, Traits<Camera>::KPtrRef camera)
{
	auto& objects = scene->getObjects();
	auto& light = scene->getLights()[0];

	// Sort objects by shader and texture to minimize state changes
	std::sort(objects.begin(), objects.end(),
		[](const Traits<Object>::Ptr& a, const Traits<Object>::Ptr& b)
		{
			if(a->getMaterial()->getShader()->getID() == b->getMaterial()->getShader()->getID())
				return a->getMaterial()->getTexture()->getID() < b->getMaterial()->getTexture()->getID();
			return a->getMaterial()->getShader()->getID() < b->getMaterial()->getShader()->getID();
		});

	// Track currently bound shader and texture
	auto shader_id = -1;
	auto texture_id = -1;

	// Draw all objects
	for (const auto& obj : objects)
	{
		auto material = obj->getMaterial();
		auto mesh = obj->getMesh();
		auto texture = material->getTexture();
		auto shader = material->getShader();

		// Bind shader and set common uniforms only if shader changed
		if(shader->getID() != shader_id)
		{
			shader_id = shader->getID();
			material->bind();
			material->setUniform("u_view", camera->getView());
			material->setUniform("u_projection", camera->getProjection());
			material->setUniform("u_view_pos", camera->getPosition());
			material->setUniform("u_light_pos", light->getPosition());
			material->setUniform("u_light_color", light->getColor());
			material->setUniform("u_sampler2d", 0);
		}
		// Bind texture only if texture changed
		if(texture->getID() != texture_id)
		{
			texture_id = texture->getID();
			texture->bind(0);
		}
		// Draw object
		material->setUniform("u_model", obj->getModel());
		material->setUniform("u_material.color", material->getColor());
		material->setUniform("u_material.ambient", material->getAmbient());
		material->setUniform("u_material.diffuse", material->getDiffuse());
		material->setUniform("u_material.specular", material->getSpecular());
		material->setUniform("u_material.shininess", material->getShininess());
		draw(mesh->getVertexArray(), shader, mesh->getDrawAs());
	}
}

}

