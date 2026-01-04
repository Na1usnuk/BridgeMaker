module;

#include "glad/glad.h"

#define GL_VERIFY(func, ...) glVerify( std::source_location::current(), func, __VA_ARGS__)
#define VERIFY(...) core::verify(std::source_location::current(), __VA_ARGS__)

module bm.gfx:renderer;

import :renderer;

import :buffer;
import :material;
import :object;
import :texture;
import :shader;
import :mesh;

import :utility;
import :verify;
import :manager;

import bm.core;

namespace bm::gfx
{
	VertexArray::VertexArray() :
		m_id(0), m_attrib_count(0)
	{
		init();
	}

	VertexArray::~VertexArray()
	{
		destroy();
	}

	VertexArray::VertexArray(VertexArray&& oth) noexcept :
		m_id(std::exchange(oth.m_id, 0)),
		m_attrib_count(std::exchange(oth.m_attrib_count, 0))
	{
	}

	VertexArray& VertexArray::operator=(VertexArray&& oth) noexcept
	{
		if (this != &oth)
		{
			destroy();
			m_id = std::exchange(oth.m_id, 0);
			m_attrib_count = std::exchange(oth.m_attrib_count, 0);
		}
		return *this;
	}

	void VertexArray::init()
	{
		GL_VERIFY(glCreateVertexArrays, 1, &m_id);
		core::log::trace("VertexArray {} created", m_id);
	}

	void VertexArray::destroy()
	{
		if (m_id != 0)
		{
			core::log::trace("VertexArray {} destroyed", m_id);
			GL_VERIFY(glDeleteVertexArrays, 1, &m_id);
			m_id = 0;
		}
	}

	void VertexArray::bind() const
	{
		VERIFY(m_id != 0, "VertexArray bind try with id = 0");
		GL_VERIFY(glBindVertexArray, m_id);
	}

	void VertexArray::unbind()
	{
		GL_VERIFY(glBindVertexArray, 0);
	}


	// Helper functions to map attribute type to OpenGL

	GLint attribSize(VertexLayout::Type type)
	{
		switch (type)
		{
		case VertexLayout::Type::Float:    return 1;
		case VertexLayout::Type::Float2:   return 2;
		case VertexLayout::Type::Float3:   return 3;
		case VertexLayout::Type::Float4:   return 4;
		case VertexLayout::Type::Int:      return 1;
		case VertexLayout::Type::Int2:     return 2;
		case VertexLayout::Type::Int3:     return 3;
		case VertexLayout::Type::Int4:     return 4;
		case VertexLayout::Type::UInt:     return 1;
		case VertexLayout::Type::UInt2:    return 2;
		case VertexLayout::Type::UInt3:    return 3;
		case VertexLayout::Type::UInt4:    return 4;
		case VertexLayout::Type::Bool:     return 1;
		}
		std::unreachable();
	}

	size_t attribByteSize(VertexLayout::Type type)
	{
		switch (type)
		{
		case VertexLayout::Type::Float:  return sizeof(GLfloat) * 1;
		case VertexLayout::Type::Float2: return sizeof(GLfloat) * 2;
		case VertexLayout::Type::Float3: return sizeof(GLfloat) * 3;
		case VertexLayout::Type::Float4: return sizeof(GLfloat) * 4;
		case VertexLayout::Type::Int:    return sizeof(GLint) * 1;
		case VertexLayout::Type::Int2:   return sizeof(GLint) * 2;
		case VertexLayout::Type::Int3:   return sizeof(GLint) * 3;
		case VertexLayout::Type::Int4:   return sizeof(GLint) * 4;
		case VertexLayout::Type::UInt:   return sizeof(GLuint) * 1;
		case VertexLayout::Type::UInt2:  return sizeof(GLuint) * 2;
		case VertexLayout::Type::UInt3:  return sizeof(GLuint) * 3;
		case VertexLayout::Type::UInt4:  return sizeof(GLuint) * 4;
		case VertexLayout::Type::Bool:   return sizeof(GLint);
		}
		std::unreachable();
	}

	GLenum attribType(VertexLayout::Type type)
	{
		switch (type)
		{
		case VertexLayout::Type::Float:
		case VertexLayout::Type::Float2:
		case VertexLayout::Type::Float3:
		case VertexLayout::Type::Float4:
			return GL_FLOAT;

		case VertexLayout::Type::Int:
		case VertexLayout::Type::Int2:
		case VertexLayout::Type::Int3:
		case VertexLayout::Type::Int4:
			return GL_INT;

		case VertexLayout::Type::UInt:
		case VertexLayout::Type::UInt2:
		case VertexLayout::Type::UInt3:
		case VertexLayout::Type::UInt4:
			return GL_UNSIGNED_INT;

		case VertexLayout::Type::Bool:
			return GL_BOOL;
		}
		std::unreachable();
	}

	GLenum topologyToGLMode(Mesh::Topology topology)
	{
		switch (topology)
		{
		case Mesh::Topology::Points: return GL_POINTS;
		case Mesh::Topology::Lines: return GL_LINES;
		case Mesh::Topology::Triangles: return GL_TRIANGLES;
		}
		std::unreachable();
	}


	void VertexArray::addVertexBuffer(const VertexBuffer& vbo, const VertexLayout& layout)
	{
		bind();
		vbo.bind();

		std::size_t offset = 0;

		for (const auto& attrib : layout.attributes)
		{
			const auto size = attribSize(attrib.type);
			const auto normalized = (attrib.normalized ? GL_TRUE : GL_FALSE);
			const auto type = attribType(attrib.type);

			GL_VERIFY(glEnableVertexAttribArray, m_attrib_count);
			GL_VERIFY(glVertexAttribPointer, m_attrib_count, size, type, normalized, 0, (void*)offset);
			offset += attribByteSize(attrib.type);
			m_attrib_count++;
		}
	}

	void VertexArray::setIndexBuffer(const IndexBuffer& ibo)
	{
		VERIFY(m_id and ibo.getId(), "Invalid VertexArray and IndexBuffer pair");
		GL_VERIFY(glVertexArrayElementBuffer, m_id, ibo.getId());
	}

	std::size_t Renderer::ShaderProgramKeyHash::operator()(const Renderer::ShaderProgramKey& key) const noexcept
	{
		HandlerHash hasher;
		std::size_t result = 0;
		for (const auto& shader : key.sources)
		{
			std::size_t h = hasher(shader);
			result ^= h + 0x9e3779b9 + (result << 6) + (result >> 2);
		}
		return result;
	}
	


	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::clearColor(std::array<float, 4> color)
	{
		glClearBufferfv(GL_COLOR, 0, color.data());
	}

	void Renderer::setViewportSize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Renderer::prepare(const Scene& scene)
	{
		auto objects = scene.getObjects();

		for (auto handler : objects)
		{
			auto optobj = ResourceManager::get(handler);
			VERIFY(optobj.has_value(), "Invalid Object");
			auto& object = optobj->get();

			if (not m_data.shapes.contains(object.getMesh()))
				makeShape(object.getMesh());

			// Ignoring material error check
			auto& material = ResourceManager::get(object.getMaterial())->get();

			auto vertex = material.getVertexShaderSource();
			auto fragment = material.getFragmentShaderSource();

			ShaderProgramKey program_key{ { vertex, fragment } };
			if (not m_data.programs.contains(program_key))
				makeShaderProgram(vertex, fragment);
		}
	}

	void Renderer::draw(const Scene& scene, Camera& camera)
	{
		auto objects = scene.getObjects();

		// Micro cache, to avoid rebind
		GLuint program_id = 0;

		for (auto handler : objects)
		{
			// If object handler is present, but object is not, scene is ill formed
			auto optobj = ResourceManager::get(handler);
			VERIFY(optobj.has_value(), "Invalid Object");
			auto& object = optobj->get();

			// Dou you know what visibility means?
			if (not object.isVisible()) continue;

			if (not m_data.shapes.contains(object.getMesh()))
				makeShape(object.getMesh());

			// Ignoring material error check
			auto& material = ResourceManager::get(object.getMaterial())->get(); 
			
			auto vertex = material.getVertexShaderSource();
			auto fragment = material.getFragmentShaderSource();

			ShaderProgramKey program_key{ { vertex, fragment } };
			if (not m_data.programs.contains(program_key))
				makeShaderProgram(vertex, fragment);

			auto& program = m_data.programs.at(program_key);
			if(program.getId() not_eq program_id)
				program.bind();
			program_id = program.getId();
			program.setUniform("u_view_projection", camera.getViewProjection());
			program.setUniform("u_model", object.transform.getModel());

			// TODO: apply material

			auto& shape = m_data.shapes.at(object.getMesh());
			shape.vao.bind();

			// Actual draw
			if (shape.ibo.has_value()) [[likely]] // I think most meshes have index buffer
			{
				shape.ibo->bind();
				GL_VERIFY(glDrawElements, shape.mode, shape.ibo->getCount(), GL_UNSIGNED_INT, (void*)0);
			}
			else
				GL_VERIFY(glDrawArrays, shape.mode, 0, shape.vbo.getSize());
		}
	}

	void Renderer::makeShape(Handler<Mesh> handler)
	{
		// Create vbo and ibo. Get layout
		auto& mesh = ResourceManager::get(handler)->get();

		VertexBuffer vbo(mesh.getVertexData(), mesh.getVertexUsage());
		std::optional<IndexBuffer> ibo;

		if (auto index_data = mesh.getIndexData())
			ibo.emplace(index_data.value(), mesh.getIndexUsage());

		auto layout = ResourceManager::get(mesh.getVertexLayout());
		VERIFY(layout.has_value(), "Invalid layout");

		// Set vao
		VertexArray vao;
		vao.addVertexBuffer(vbo, layout.value());
		if (ibo)
			vao.setIndexBuffer(ibo.value());

		GLenum mode = topologyToGLMode(mesh.getTopology());

		// Save it
		m_data.shapes.emplace(handler, Shape(std::move(vao), std::move(vbo), std::move(ibo), mode));
	}

	void Renderer::makeShader(Handler<ShaderSource> handler, Shader::Stage stage)
	{
		auto optsrc = ResourceManager::get(handler);
		VERIFY(optsrc.has_value(), "Invalid shader source");
		auto& src = optsrc->get();

		Shader shader(src, stage);

		m_data.shaders.emplace(handler, std::move(shader));
	}

	void Renderer::makeShaderProgram(Handler<ShaderSource> vertex, Handler<ShaderSource> fragment)
	{
		if (not m_data.shaders.contains(vertex))
			makeShader(vertex, Shader::Stage::Vertex);
		if (not m_data.shaders.contains(fragment))
			makeShader(fragment, Shader::Stage::Fragment);

		auto& vertex_shader = m_data.shaders.at(vertex);
		auto& fragment_shader = m_data.shaders.at(fragment);

		ShaderProgramKey key{ { vertex, fragment } };
		ShaderProgram program(vertex_shader, fragment_shader);

		m_data.programs.emplace(std::move(key), std::move(program));
	}

}

//
//
//	static constexpr std::string_view vertex2d_src =
//		R"(
//			#version 330 core
//
//			layout(location = 0) in vec3 a_pos;
//			layout(location = 1) in vec2 a_tex;
//			layout(location = 2) in vec4 a_color;
//			layout(location = 3) in float a_slot;
//
//			uniform mat4 u_projection;
//			uniform mat4 u_view;
//
//			out vec2 f_tex;
//			out vec4 f_color;
//			flat out int f_slot;
//
//			void main() 
//			{
//			    gl_Position = u_projection * u_view * vec4(a_pos, 1.0);
//				f_tex = a_tex;
//				f_color = a_color;
//				f_slot = int(a_slot);
//			}	
//		)";
//
//	static constexpr std::string_view fragment2d_src =
//		R"(
//			#version 330 core
//
//			in  vec2 f_tex;
//			in  vec4 f_color;
//			flat in int f_slot;
//
//			out vec4 o_color;
//
//			uniform sampler2D u_sampler[32];
//
//			void main() 
//			{
//				o_color = texture(u_sampler[f_slot], f_tex) * f_color;
//			}
//		)";
//
//	Quad::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Traits<Texture>::KSPtrRef texture) :
//		m_pos(position),
//		m_size(size),
//		m_color(color),
//		m_tex(texture)
//	{
//		if (m_tex == nullptr)
//			m_tex = Manager::get().loadTexture("nothing");
//	}
//	Quad::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, Traits<Texture>::KSPtrRef texture) :
//		Quad(glm::vec3(position.x, position.y, 1.f), size, color, texture)
//	{
//	}
//
//	std::array<unsigned int, ScreenRenderer::Data::max_indices> ScreenRenderer::Data::makeIndices()
//	{
//		// Static indices for every quad
//		std::array<unsigned int, max_indices> indices;
//		std::size_t offset = 0;
//		for (std::size_t i = 0; i < indices.size(); i += 6)
//		{
//			// First triangle
//			indices[i + 0] = offset + 0;
//			indices[i + 1] = offset + 1;
//			indices[i + 2] = offset + 2;
//			// Second triangle
//			indices[i + 3] = offset + 0;
//			indices[i + 4] = offset + 2;
//			indices[i + 5] = offset + 3;
//
//			offset += 4;
//		}
//		return indices;
//	}
//
//	ScreenRenderer::Data::Data() :
//		vao(VertexBuffer(max_vertices * sizeof(QuadVertex), Usage::Stream), IndexBuffer(makeIndices())),
//		shader(Shader::make(vertex2d_src, fragment2d_src)),
//		vertices()
//	{
//		// Bind shader and set texture samplers
//		shader->bind();
//
//		int samplers[32];
//		for (int i = 0; i < 32; i++)
//			samplers[i] = i;
//
//		shader->setUniform("u_sampler", samplers, 32);
//
//		
//		// Why you read this comment? Dont know what it is? 
//		vao.setLayout
//		({
//			{Shader::Type::Float3, "a_pos"},
//			{Shader::Type::Float2, "a_tex"},
//			{Shader::Type::Float4, "a_color"},
//			{Shader::Type::Float,  "a_slot"}
//		});
//	}
//
//	void ScreenRenderer::submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Traits<Texture>::KSPtrRef texture)
//	{
//		// Flush if we exceed batch limits
//		if ((m_data.quad_count >= m_data.max_quads) or (texture && m_data.texture_slot_index >= m_data.max_texture_slots))
//			draw();
//
//		// Texture slot management
//		float texture_index = 0.0f; // 0 = white texture
//		if (texture)
//		{
//			bool found = false;
//			for (std::size_t i = 1; i < m_data.texture_slot_index; i++)
//			{
//				if (m_data.texture_slots[i] == texture)
//				{
//					texture_index = static_cast<float>(i);
//					found = true;
//					break;
//				}
//			}
//			if (!found)
//			{
//				texture_index = static_cast<float>(m_data.texture_slot_index);
//				m_data.texture_slots[m_data.texture_slot_index] = texture;
//				m_data.texture_slot_index++;
//				texture->bind(static_cast<std::uint32_t>(texture_index));
//			}
//		}
//
//		// Define vertices for quad
//		auto count = m_data.quad_count * 4;
//
//		m_data.vertices[count + 0] =
//		{
//			position,
//			{0.0f, 0.0f},
//			color,
//			texture_index
//		};
//		m_data.vertices[count + 1] =
//		{
//			{ position.x + size.x, position.y, position.z },
//			{1.0f, 0.0f},
//			color,
//			texture_index
//		};
//		m_data.vertices[count + 2] =
//		{
//			{ position.x + size.x, position.y + size.y, position.z},
//			{1.0f, 1.0f},
//			color,
//			texture_index
//		};
//		m_data.vertices[count + 3] =
//		{
//			{ position.x, position.y + size.y, position.z},
//			{0.0f, 1.0f},
//			color,
//			texture_index
//		};
//
//		m_data.quad_count++;
//
//	}
//
//	void ScreenRenderer::draw()
//	{
//		m_data.vao.bind();
//		m_data.shader->bind();
//
//		// Camera needs to be set at this point 
//		VERIFY(bool(m_camera), "Camera not set for ScreenRenderer. Please call ScreenRenderer::setCamera");
//
//		m_data.shader->setUniform("u_view", m_camera->getView());
//		m_data.shader->setUniform("u_projection", m_camera->getProjection());
//
//		m_data.vao.getVertexBuffer().setData(m_data.vertices.data(), sizeof(QuadVertex) * 4 * m_data.quad_count);
//		GL_VERIFY(glDrawElements, GL_TRIANGLES, m_data.quad_count * 6, GL_UNSIGNED_INT, nullptr);
//		m_data.quad_count = 0;
//		m_data.texture_slot_index = 1;
//	}
//
//}
//