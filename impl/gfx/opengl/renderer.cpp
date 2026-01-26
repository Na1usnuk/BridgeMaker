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

	GLint attribSize(DataType type)
	{
		switch (type)
		{
		case DataType::Float:
		case DataType::Double:
		case DataType::Int:
		case DataType::UInt:
		case DataType::Bool:
			return 1;

		case DataType::Vec2:
		case DataType::DVec2:
		case DataType::IVec2:
		case DataType::UVec2:
		case DataType::BVec2:
			return 2;

		case DataType::Vec3:
		case DataType::DVec3:
		case DataType::IVec3:
		case DataType::UVec3:
		case DataType::BVec3:
			return 3;

		case DataType::Vec4:
		case DataType::DVec4:
		case DataType::IVec4:
		case DataType::UVec4:
		case DataType::BVec4:
			return 4;
		}

		std::unreachable();
	}

	size_t attribByteSize(DataType type)
	{
		switch (type)
		{
			// float
		case DataType::Float:  return sizeof(GLfloat) * 1;
		case DataType::Vec2:   return sizeof(GLfloat) * 2;
		case DataType::Vec3:   return sizeof(GLfloat) * 3;
		case DataType::Vec4:   return sizeof(GLfloat) * 4;

			// double
		case DataType::Double: return sizeof(GLdouble) * 1;
		case DataType::DVec2:  return sizeof(GLdouble) * 2;
		case DataType::DVec3:  return sizeof(GLdouble) * 3;
		case DataType::DVec4:  return sizeof(GLdouble) * 4;

			// signed int
		case DataType::Int:    return sizeof(GLint) * 1;
		case DataType::IVec2:  return sizeof(GLint) * 2;
		case DataType::IVec3:  return sizeof(GLint) * 3;
		case DataType::IVec4:  return sizeof(GLint) * 4;

			// unsigned int
		case DataType::UInt:   return sizeof(GLuint) * 1;
		case DataType::UVec2:  return sizeof(GLuint) * 2;
		case DataType::UVec3:  return sizeof(GLuint) * 3;
		case DataType::UVec4:  return sizeof(GLuint) * 4;

			// bools
		case DataType::Bool:   return sizeof(GLint) * 1;
		case DataType::BVec2:  return sizeof(GLint) * 2;
		case DataType::BVec3:  return sizeof(GLint) * 3;
		case DataType::BVec4:  return sizeof(GLint) * 4;
		}

		std::unreachable();
	}

	GLenum attribType(DataType type)
	{
		switch (type)
		{
			// float
		case DataType::Float:
		case DataType::Vec2:
		case DataType::Vec3:
		case DataType::Vec4:
			return GL_FLOAT;

			// double
		case DataType::Double:
		case DataType::DVec2:
		case DataType::DVec3:
		case DataType::DVec4:
			return GL_DOUBLE;

			// signed int
		case DataType::Int:
		case DataType::IVec2:
		case DataType::IVec3:
		case DataType::IVec4:
			return GL_INT;

			// unsigned int
		case DataType::UInt:
		case DataType::UVec2:
		case DataType::UVec3:
		case DataType::UVec4:
			return GL_UNSIGNED_INT;

			// bools are integer attributes in OpenGL
		case DataType::Bool:
		case DataType::BVec2:
		case DataType::BVec3:
		case DataType::BVec4:
			return GL_BOOL;
		}

		std::unreachable();
	}
	
	template<DataType>
	struct ShaderToCppType
	{
		static_assert(false, "Unsupported DataType");
	};

	template<> struct ShaderToCppType<DataType::Float> { using Type = float; };
	template<> struct ShaderToCppType<DataType::Vec2> { using Type = glm::vec2; };
	template<> struct ShaderToCppType<DataType::Vec3> { using Type = glm::vec3; };
	template<> struct ShaderToCppType<DataType::Vec4> { using Type = glm::vec4; };

	template<> struct ShaderToCppType<DataType::Double> { using Type = double; };
	template<> struct ShaderToCppType<DataType::DVec2> { using Type = glm::dvec2; };
	template<> struct ShaderToCppType<DataType::DVec3> { using Type = glm::dvec3; };
	template<> struct ShaderToCppType<DataType::DVec4> { using Type = glm::dvec4; };

	template<> struct ShaderToCppType<DataType::Int> { using Type = int; };
	template<> struct ShaderToCppType<DataType::IVec2> { using Type = glm::ivec2; };
	template<> struct ShaderToCppType<DataType::IVec3> { using Type = glm::ivec3; };
	template<> struct ShaderToCppType<DataType::IVec4> { using Type = glm::ivec4; };

	template<> struct ShaderToCppType<DataType::UInt> { using Type = unsigned int; };
	template<> struct ShaderToCppType<DataType::UVec2> { using Type = glm::uvec2; };
	template<> struct ShaderToCppType<DataType::UVec3> { using Type = glm::uvec3; };
	template<> struct ShaderToCppType<DataType::UVec4> { using Type = glm::uvec4; };

	template<> struct ShaderToCppType<DataType::Bool> { using Type = int; };
	template<> struct ShaderToCppType<DataType::BVec2> { using Type = glm::ivec2; };
	template<> struct ShaderToCppType<DataType::BVec3> { using Type = glm::ivec3; };
	template<> struct ShaderToCppType<DataType::BVec4> { using Type = glm::ivec4; };

	template<DataType T>
	using ShaderCppType = typename ShaderToCppType<T>::Type;


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
		m_attrib_count = 0;

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

	void VertexArray::unsetIndexBuffer()
	{
		VERIFY(m_id, "Invalid VertexArray");
		GL_VERIFY(glVertexArrayElementBuffer, m_id, 0);
	}

	std::size_t Renderer::ShaderProgramKeyHash::operator()(const Renderer::ShaderProgramKey& key) const noexcept
	{
		core::HandleHash hasher;
		std::size_t result = 0;
		for (const auto& shader : key.sources)
		{
			std::size_t h = hasher(shader);
			result ^= h + 0x9e3779b9 + (result << 6) + (result >> 2);
		}
		return result;
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
		const auto object_handles = scene.getObjects();

		for (const auto object_handle : object_handles)
			prepare(object_handle);
	}

	void Renderer::prepare(const core::Handle<Object> object_handle)
	{
		const auto optobj = m_manager.objects.tryGet(object_handle);
		VERIFY(optobj.has_value(), "Invalid Object");
		const auto& object = optobj->get();
		const auto& mesh = object.getMesh();
		const auto& material = object.getMaterial();

		if (not m_renderables.contains(object_handle))
		{
			m_renderables.emplace(object_handle, Renderable{ mesh, material, object.transform, object.isVisible() });
			prepare(mesh);
			prepare(material);
		}
	}

	void Renderer::prepare(const core::Handle<Mesh> mesh_handle)
	{
		const auto optmesh = m_manager.meshes.tryGet(mesh_handle);
		VERIFY(optmesh.has_value(), "Invalid Mesh");
		const auto& mesh = optmesh->get();

		if (not m_shapes.contains(mesh_handle))
			allocate(mesh_handle);

		auto& shape = m_shapes.at(mesh_handle);

		if (shape.version not_eq mesh.getVersion())
			update(mesh_handle);
	}

	void Renderer::prepare(const core::Handle<Material> material_handle)
	{
		const auto optmat = m_manager.materials.tryGet(material_handle);
		VERIFY(optmat.has_value(), "Invalid Material");
		const auto& material = optmat->get();
		auto& appearance = m_appearances[material_handle];

		auto& cached_version = appearance.version;
		const auto& version = material.getVersion();

		// Check shaders
		const auto vertex_source = material.getVertexShaderSource();
		const auto fragment_source = material.getFragmentShaderSource();

		// Check vertex shader validity
		if(cached_version.vertex_source not_eq version.vertex_source)
		{
			if (not m_shaders.contains(vertex_source))
				allocate(vertex_source, ShaderSource::Stage::Vertex);
			cached_version.vertex_source = version.vertex_source;
		}

		// Check fragment shader validity
		if (cached_version.fragment_source not_eq version.fragment_source)
		{
			if (not m_shaders.contains(fragment_source))
				allocate(fragment_source, ShaderSource::Stage::Fragment);
			cached_version.fragment_source = version.fragment_source;
		}

		// Check shader program
		ShaderProgramKey program_key{ vertex_source, fragment_source };
		if(not m_programs.contains(program_key))
		{
			allocate(vertex_source, fragment_source);
			appearance.program = program_key;
		}
		auto& program = m_programs.at(program_key);

		// Check uniforms
		if (appearance.bindings.getVersion() not_eq material.bindings.getVersion())
		{
			// TODO: Optimize
			appearance.bindings = material.bindings;
			
		}
		cached_version = version;
	}

	void Renderer::prepare(const core::Handle<Image> image_handle)
	{
		if (not m_textures.contains(image_handle))
			allocate(image_handle);
	}

	void Renderer::destroy(core::Handle<Object> object_handle)
	{
		// Nothing to do for now
	}

	void Renderer::destroy(core::Handle<Mesh> mesh_handle)
	{
		m_shapes.erase(mesh_handle);
	}

	void Renderer::destroy(core::Handle<ShaderSource> shader_source_handle)
	{
		m_shaders.erase(shader_source_handle);
		// Also erase programs that use this shader
		// Or not?
		//for (auto it = m_programs.begin(); it != m_programs.end(); )
		//{
		//	const auto& key = it->first;
		//	if (core::contains(key.sources, shader_source_handle))
		//		it = m_programs.erase(it);
		//	else
		//		++it;
		//}
	}

	void Renderer::destroy(core::Handle<Material> material_handle)
	{
		// Nothing to do for now
	}

	void Renderer::destroy(core::Handle<Image> image_handle)
	{
		m_textures.erase(image_handle);
	}

	void Renderer::destroy()
	{
		m_shapes.clear();
		m_textures.clear();
		m_shaders.clear();
		m_programs.clear();
	}

	void Renderer::allocate(const core::Handle<Mesh> mesh_handle)
	{
		// Prepare already checked for mesh existence, so no need to check again
		const auto& mesh = m_manager.meshes.get(mesh_handle);

		// Create buffers
		VertexBuffer vbo(mesh.getVertexData(), mesh.getVertexUsage());

		std::optional<IndexBuffer> ibo;
		if (auto index_data = mesh.getIndexData())
			ibo.emplace(index_data.value(), mesh.getIndexUsage());

		// Layout must be managed by ResourceManager
		const auto& layout = m_manager.vertex_layouts.get(mesh.getVertexLayout());

		// Create VAO
		VertexArray vao;
		vao.addVertexBuffer(vbo, layout);
		if (ibo)
			vao.setIndexBuffer(ibo.value());

		int mode = topologyToGLMode(mesh.getTopology());

		Shape shape =
		{
			.version = mesh.getVersion(),
			.vao = std::move(vao),
			.vbo = std::move(vbo),
			.ibo = std::move(ibo),
			.mode = mode
		};

		// Save it
		m_shapes.emplace(mesh_handle, std::move(shape));
	}

	void Renderer::allocate(core::Handle<ShaderSource> handler, ShaderSource::Stage stage)
	{
		auto optsrc = m_manager.shader_sources.tryGet(handler);
		VERIFY(optsrc.has_value(), "Invalid shader source");
		auto& src = optsrc->get();
		VERIFY(src.getStage() == stage, "Invalid shader source stage");

		Shader shader(src);

		m_shaders.emplace(handler, std::move(shader));
	}

	void Renderer::allocate(core::Handle<ShaderSource> vertex_handle, core::Handle<ShaderSource> fragment_handle)
	{
		// At this point, shaders must be already allocated
		auto& vertex_shader = m_shaders.at(vertex_handle);
		auto& fragment_shader = m_shaders.at(fragment_handle);
		
		ShaderProgramKey key{ { vertex_handle, fragment_handle } };
	
		ShaderProgram program(vertex_shader, fragment_shader);
		
		m_programs.emplace(std::move(key), std::move(program));
	}

	void Renderer::allocate(core::Handle<Image> image_handle)
	{
		const auto optimg = m_manager.images.tryGet(image_handle);
		VERIFY(optimg.has_value(), "Invalid Image");
		const auto& image = optimg->get();

		Texture texture(image);

		m_textures.emplace(image_handle, std::move(texture));
	}

	void Renderer::update(const core::Handle<Mesh> mesh_handle)
	{
		// Prepare already checked for mesh existence, so no need to check again
		const auto& mesh = m_manager.meshes.get(mesh_handle);
		// Shape must be already allocated
		auto& shape = m_shapes.at(mesh_handle);

		// Update buffers if needed
		if (mesh.getVersion().vertex not_eq shape.version.vertex)
			shape.vbo.setData(mesh.getVertexData());

		// Update index buffer
		if (mesh.getVersion().index not_eq shape.version.index)
		{
			if (auto index_data = mesh.getIndexData())
				shape.ibo->setData(index_data.value());
			else
			{
				shape.ibo.reset();
				shape.vao.unsetIndexBuffer(); // Unset ibo
			}
		}

		// Update layout
		if (mesh.getVersion().layout not_eq shape.version.layout)
		{
			const auto& layout = m_manager.vertex_layouts.get(mesh.getVertexLayout());
			shape.vao.bind();
			shape.vao.addVertexBuffer(shape.vbo, layout);
		}

		shape.version = mesh.getVersion();
	}

	void Renderer::draw(const Scene& scene, Camera& camera)
	{
		FrameContext frame_ctx{ camera, {} };
		m_on_frame(frame_ctx);

		auto objects = scene.getObjects();

		for (auto object : objects)
		{
			auto& renderable = m_renderables.at(object);

			ObjectContext object_ctx{ renderable, {} };
			m_on_object(object_ctx);

			// Dou you know what visibility means?
			if (not renderable.is_visible) continue;

			auto& shape = m_shapes.at(renderable.mesh);
			auto& appearance = m_appearances.at(renderable.material);

			// For now just dumb rendereing
			auto& program = m_programs.at(appearance.program);
			program.bind();

			auto bind_bindings = [&](const auto& bindings)
				{
					for (auto& binding : bindings.getBindings())
						std::visit([&](const auto& data) { program.setUniform(binding.name, data); }, binding.data);
				};

			bind_bindings(frame_ctx.bindings);
			bind_bindings(object_ctx.bindings);
			bind_bindings(appearance.bindings);
			
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


}