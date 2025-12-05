module;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
#include "gl_call.hpp"

module bm.gfx.screen_renderer;

import bm.app;
import bm.gfx.renderer;
import bm.gfx.utility;
import bm.log;
import bm.verify;

import std;

namespace bm::gfx
{

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
		auto vbo(VertexBuffer::make(max_vertices * sizeof(QuadVertex), VertexBuffer::Usage::Dynamic)); 

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
