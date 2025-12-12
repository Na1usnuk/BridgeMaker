module;

#include "glad/glad.h"
#include "gl_call.hpp"

module bm.gfx:renderer;
import :renderer;

import :utility;
import :buffer;
import :array;
import :shader;
import :asset_manager;


import std;


namespace bm::gfx
{
	
	Renderer::Renderer()
	{
		glCall(glGetIntegerv, GL_MAX_TEXTURE_IMAGE_UNITS, &m_state_cache.texture_slot_count);
		m_state_cache.bound_textures.resize(m_state_cache.texture_slot_count, -1);
	
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
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
	
	void Renderer::setDepthWrite(bool b)
	{
		glDepthMask((b ? GL_TRUE : GL_FALSE));
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
	
		if(vao->getIndexBuffer() != nullptr) [[likely]]
			glCall(glDrawElements, static_cast<int>(draw_as), vao->getIndexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
		else
			glCall(glDrawArrays, static_cast<int>(draw_as), 0, vao->getVerticesCount());
	}
	
	
	void Renderer::draw(Scene& scene, Camera& camera)
	{
		auto& objects = scene.getObjects();
		auto& light = scene.getLights()[0];
	
		// 1. Sort all objects by shader/texture for opaque pass
		std::sort(objects.begin(), objects.end(),
			[](const Traits<Object>::Ptr& a, const Traits<Object>::Ptr& b)
			{
				auto ma = a->getMaterial();
				auto mb = b->getMaterial();
	
				GLuint sa = ma->getShader()->getID();
				GLuint sb = mb->getShader()->getID();
	
				if (sa != sb)
					return sa < sb;
	
				GLuint ta = ma->getTexture() ? ma->getTexture()->getID() : 0;
				GLuint tb = mb->getTexture() ? mb->getTexture()->getID() : 0;
				if (ta != tb)
					return ta < tb;
	
				GLuint na = ma->getNormalMap() ? ma->getNormalMap()->getID() : 0;
				GLuint nb = mb->getNormalMap() ? mb->getNormalMap()->getID() : 0;
				return na < nb;
			});
	
	
		// 2. Opaque pass
		int shader_id = -1;
		int texture_id = -1;
		int normal_id = -1;
	
		auto draw_impl = [&](auto& obj)
			{
				auto material = obj->getMaterial();
				auto mesh = obj->getMesh();
				auto texture = material->getTexture();
				auto normalMap = material->getNormalMap();
				auto shader = material->getShader();
	
				if (shader->getID() != shader_id)
				{
					shader_id = shader->getID();
					material->bind();
					material->setUniform("u_view", camera.getView());
					material->setUniform("u_projection", camera.getProjection());
					material->setUniform("u_view_pos", camera.getPosition());
					material->setUniform("u_light_pos", light->getPosition());
					material->setUniform("u_light_color", light->getColor());
					material->setUniform("u_sampler2d", 0 + 16);      // albedo
					material->setUniform("u_normal_map", 1 + 16);      // NEW
				}
	
				if (texture && texture->getID() != texture_id)
				{
					texture_id = texture->getID();
					texture->bind(0 + 16);
				}
	
				if (normalMap && normalMap->getID() != normal_id)
				{
					normal_id = normalMap->getID();
					normalMap->bind(1 + 16); // texture unit 1
				}
	
				material->setUniform("u_has_normal_map", normalMap != nullptr);
	
				material->setUniform("u_model", obj->getTransform().getModel());
				material->setUniform("u_material.color", material->getColor());
				material->setUniform("u_material.ambient", material->getAmbient());
				material->setUniform("u_material.diffuse", material->getDiffuse());
				material->setUniform("u_material.specular", material->getSpecular());
				material->setUniform("u_material.shininess", material->getShininess());
	
				draw(mesh->getVertexArray(), shader, mesh->getDrawAs());
			};
	
		// draw opaque
		for (auto& obj : objects)
		{
			if (obj->getMaterial()->getColor()[3] == 1.f)
				draw_impl(obj);
		}
	
		// 3. Build transparent index list
		std::vector<std::size_t> transparent;
		transparent.reserve(objects.size());
	
		for (std::size_t i = 0; i < objects.size(); ++i)
		{
			if (objects[i]->getMaterial()->getColor()[3] < 1.f)
				transparent.push_back(i);
		}
	
		// 4. Sort transparent back-to-front
		std::ranges::sort(transparent, [&](std::size_t a, std::size_t b) {
			float da = glm::distance(camera.getPosition(), objects[a]->getTransform().getPosition());
			float db = glm::distance(camera.getPosition(), objects[b]->getTransform().getPosition());
			return da > db; // far first
			});
	
	
		// 5. Transparent pass
		setDepthWrite(false);
		for (std::size_t i : transparent)
			draw_impl(objects[i]);
		setDepthWrite(true);
	
	
	}


	static constexpr std::string_view vertex2d_src =
		R"(
			#version 330 core

			layout(location = 0) in vec3 a_pos;
			layout(location = 1) in vec2 a_tex;
			layout(location = 2) in vec4 a_color;
			layout(location = 3) in float a_slot;

			uniform mat4 u_projection;
			uniform mat4 u_view;

			out vec2 f_tex;
			out vec4 f_color;
			flat out int f_slot;

			void main() 
			{
			    gl_Position = u_projection * u_view * vec4(a_pos, 1.0);
				f_tex = a_tex;
				f_color = a_color;
				f_slot = int(a_slot);
			}	
		)";

	static constexpr std::string_view fragment2d_src =
		R"(
			#version 330 core

			in  vec2 f_tex;
			in  vec4 f_color;
			flat in int f_slot;

			out vec4 o_color;

			uniform sampler2D u_sampler[32];

			void main() 
			{
				o_color = texture(u_sampler[f_slot], f_tex) * f_color;
			}
		)";

	Quad::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Traits<Texture>::KSPtrRef texture) :
		m_pos(position),
		m_size(size),
		m_color(color),
		m_tex(texture)
	{
		if (m_tex == nullptr)
			m_tex = AssetManager::get().loadTexture("nothing");
	}
	Quad::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, Traits<Texture>::KSPtrRef texture) :
		Quad(glm::vec3(position.x, position.y, 1.f), size, color, texture)
	{
	}

	ScreenRenderer::Data::Data() :
		vao(VertexArray::make()),
		shader(Shader::make(vertex2d_src, fragment2d_src)),
		vertices()
	{
		// Bind shader and set texture samplers
		shader->bind();

		int samplers[32];
		for (int i = 0; i < 32; i++)
			samplers[i] = i;

		shader->setUniform("u_sampler", samplers, 32);


		// Allocate buffer for max amount of vertices
		auto vbo(VertexBuffer::make(max_vertices * sizeof(QuadVertex), Usage::Stream));

		// Static indices for every quad
		std::array<unsigned int, max_indices> indices;
		std::size_t offset = 0;
		for (std::size_t i = 0; i < indices.size(); i += 6)
		{
			// First triangle
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			// Second triangle
			indices[i + 3] = offset + 0;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;

			offset += 4;
		}
		auto ibo(IndexBuffer::make(indices));

		// Why you read this comment? Dont know what it is? 
		vbo->setLayout
		({
			{Shader::Type::Float3, "a_pos"},
			{Shader::Type::Float2, "a_tex"},
			{Shader::Type::Float4, "a_color"},
			{Shader::Type::Float,  "a_slot"}
			});

		// Give ownership to VertexArray
		vao->setVertexBuffer(std::move(vbo));
		vao->setIndexBuffer(std::move(ibo));
	}

	void ScreenRenderer::submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Traits<Texture>::KSPtrRef texture)
	{
		// Flush if we exceed batch limits
		if ((m_data.quad_count >= m_data.max_quads) or (texture && m_data.texture_slot_index >= m_data.max_texture_slots))
			draw();

		// Texture slot management
		float texture_index = 0.0f; // 0 = white texture
		if (texture)
		{
			bool found = false;
			for (std::size_t i = 1; i < m_data.texture_slot_index; i++)
			{
				if (m_data.texture_slots[i] == texture)
				{
					texture_index = static_cast<float>(i);
					found = true;
					break;
				}
			}
			if (!found)
			{
				texture_index = static_cast<float>(m_data.texture_slot_index);
				m_data.texture_slots[m_data.texture_slot_index] = texture;
				m_data.texture_slot_index++;
				texture->bind(static_cast<std::uint32_t>(texture_index));
			}
		}

		// Define vertices for quad
		auto count = m_data.quad_count * 4;

		m_data.vertices[count + 0] =
		{
			position,
			{0.0f, 0.0f},
			color,
			texture_index
		};
		m_data.vertices[count + 1] =
		{
			{ position.x + size.x, position.y, position.z },
			{1.0f, 0.0f},
			color,
			texture_index
		};
		m_data.vertices[count + 2] =
		{
			{ position.x + size.x, position.y + size.y, position.z},
			{1.0f, 1.0f},
			color,
			texture_index
		};
		m_data.vertices[count + 3] =
		{
			{ position.x, position.y + size.y, position.z},
			{0.0f, 1.0f},
			color,
			texture_index
		};

		m_data.quad_count++;

	}

	void ScreenRenderer::draw()
	{
		m_data.vao->bind();
		m_data.shader->bind();

		// Camera needs to be set at this point 
		core::verify(bool(m_camera), "Camera not set for ScreenRenderer. Please call ScreenRenderer::setCamera");

		m_data.shader->setUniform("u_view", m_camera->getView());
		m_data.shader->setUniform("u_projection", m_camera->getProjection());

		m_data.vao->getVertexBuffer()->setData(m_data.vertices.data(), sizeof(QuadVertex) * 4 * m_data.quad_count);
		glCall(glDrawElements, GL_TRIANGLES, m_data.quad_count * 6, GL_UNSIGNED_INT, nullptr);
		m_data.quad_count = 0;
		m_data.texture_slot_index = 1;
	}

}

