module;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

module bm.gfx.screen_renderer;

import bm.app;
import bm.gfx.renderer;
import bm.gfx.utility;
import bm.log;

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
			out float f_slot;

			void main() 
			{
			    gl_Position = u_projection * u_view * vec4(a_pos, 1.0);
				f_tex = a_tex;
				f_color = a_color;
				f_slot = a_slot;
			}	
		)";

	static constexpr std::string_view fragment2d_src =
		R"(
			#version 330 core

			in  vec2 f_tex;
			in  vec4 f_color;
			in float f_slot;

			out vec4 o_color;

			uniform sampler2D u_sampler;

			void main() 
			{
				o_color = f_color;
			}
		)";

	//static constexpr std::array<float, 5 * 4> vertices =
	//{
	//	// x      y      z     u     v 
	//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left
	//	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
	//	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top-right
	//	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top-left
	//};

	//static constexpr std::array<unsigned int, 6> indices =
	//{
	//	0, 1, 2,
	//	0, 2, 3
	//};

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec4 color;
		float texslot;
	};

	ScreenRenderer::Data::Data() :
		vao(VertexArray::make()),
		shader(Shader::make(vertex2d_src, fragment2d_src))
	{
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
		log::core::trace("Submit quad at ({}, {}, {}) size ({}, {}) color ({}, {}, {}, {})",
			position.x, position.y, position.z, size.x, size.y,
			color.r, color.g, color.b, color.a);

		if (m_data.quad_count >= m_data.max_quads)
		{
			// For now you need to manually control you quad count, and call draw if it >= max_quads
			log::core::error("Max quad amount ({}) overflow", m_data.max_quads);
			return;
		}

		std::array<QuadVertex, 4> quad_data;

		quad_data[0] = 
		{ 
			position, 
			{0.0f, 0.0f}, 
			color, 
			0 
		};
		quad_data[1] =
		{
			{ position.x + size.x, position.y, position.z },
			{1.0f, 0.0f},
			color,
			0
		};
		quad_data[2] =
		{
			{ position.x + size.x, position.y + size.y, position.z},
			{1.0f, 1.0f},
			color,
			0
		};
		quad_data[3] =
		{
			{ position.x, position.y + size.y, position.z},
			{0.0f, 1.0f},
			color,
			0
		};

		m_data.vao->getVertexBuffer()->populate(quad_data.data(), sizeof(QuadVertex) * 4, m_data.quad_count * sizeof(QuadVertex) * 4);
		m_data.quad_count++;

	}

	void ScreenRenderer::draw(Traits<ScreenCamera>::KPtrRef camera)
	{
		log::core::trace("Drawing {} quads", m_data.quad_count);

		glDisable(GL_DEPTH_TEST);  // Disable depth test for 2D
		glDisable(GL_CULL_FACE);   // Disable backface culling
		glEnable(GL_BLEND);        // Enable blending for transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_data.vao->bind();
		m_data.shader->bind();

		m_data.shader->setUniform("u_view", camera->getView());
		m_data.shader->setUniform("u_projection", camera->getProjection());
		//m_data.shader->setUniform("u_model", glm::mat4(1.f));


		glCall(glDrawElements, GL_TRIANGLES, m_data.quad_count * 6, GL_UNSIGNED_INT, nullptr);
		m_data.quad_count = 0;

		glEnable(GL_DEPTH_TEST);

	}

}
