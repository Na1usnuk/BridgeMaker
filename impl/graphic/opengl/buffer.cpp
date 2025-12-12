module bm.gfx:buffer;

import :utility;
import :buffer;
import :shader;
import :context;

import bm.core;
import bm.platform;

import <glad/glad.h>;
import <gl_call.hpp>;

namespace bm::gfx
{

	VertexBuffer::Layout::Element::Element(Shader::Type type_, std::string_view name_, bool normalized_) :
		type(type_),
		gl_type(getGLType(type_)),
		name(name_),
		size(getSize(type_)),
		count(getComponentCount(type_)),
		offset(0),
		normalized(normalized_)
	{
	}

	VertexBuffer::Layout::Layout() : m_stride(0) {}
	VertexBuffer::Layout::Layout(const std::initializer_list<Element>& layout) :
		m_elements(layout),
		m_stride(0)
	{
		calculateOffsetsAndStride();
	}

	void VertexBuffer::Layout::calculateOffsetsAndStride()
	{
		size_t offset = 0;
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}

	constexpr std::uint32_t VertexBuffer::Layout::Element::getSize(Shader::Type type)
	{
		switch (type)
		{
		case Shader::Type::Float:		return sizeof(GLfloat);
		case Shader::Type::Float2:		return sizeof(GLfloat) * 2;
		case Shader::Type::Float3:		return sizeof(GLfloat) * 3;
		case Shader::Type::Float4:		return sizeof(GLfloat) * 4;
		case Shader::Type::Float3x3:    return sizeof(GLfloat) * 3 * 3;
		case Shader::Type::Float4x4:    return sizeof(GLfloat) * 4 * 4;
		case Shader::Type::Int:			return sizeof(GLint);
		case Shader::Type::Int2:		return sizeof(GLint) * 2;
		case Shader::Type::Int3:		return sizeof(GLint) * 3;
		case Shader::Type::Int4:		return sizeof(GLint) * 4;
		case Shader::Type::Bool:		return sizeof(GLboolean);
		}
		core::verify(false, "Not existing VertexBufferLayout::Element type");
		return 0;
	}

	constexpr std::int32_t VertexBuffer::Layout::Element::getComponentCount(Shader::Type type)
	{
		switch (type)
		{
		case Shader::Type::Float:		return 1;
		case Shader::Type::Float2:		return 2;
		case Shader::Type::Float3:		return 3;
		case Shader::Type::Float4:		return 4;
		case Shader::Type::Float3x3:    return 3 * 3;
		case Shader::Type::Float4x4:    return 4 * 4;
		case Shader::Type::Int:			return 1;
		case Shader::Type::Int2:		return 2;
		case Shader::Type::Int3:		return 3;
		case Shader::Type::Int4:		return 4;
		case Shader::Type::Bool:		return 1;
		}
		core::verify(false, "Not existing VertexBufferLayout::Element type");
		return 0;
	}

	constexpr std::uint32_t VertexBuffer::Layout::Element::getGLType(Shader::Type type)
	{
		switch (type)
		{
			// Float types
		case Shader::Type::Float:
		case Shader::Type::Float2:
		case Shader::Type::Float3:
		case Shader::Type::Float4:
		case Shader::Type::Float2x2:
		case Shader::Type::Float3x3:
		case Shader::Type::Float4x4: return GL_FLOAT;

			// Signed integers
		case Shader::Type::Int:
		case Shader::Type::Int2:
		case Shader::Type::Int3:
		case Shader::Type::Int4:    return GL_INT;

			// Unsigned integers
		case Shader::Type::UInt:
		case Shader::Type::UInt2:
		case Shader::Type::UInt3:
		case Shader::Type::UInt4:   return GL_UNSIGNED_INT;

			// Boolean
		case Shader::Type::Bool:    return GL_BOOL;
		}
		core::verify(false, "Not existing VertexBufferLayout::Element type");
		return 0;
	}

	VertexBuffer::VertexBuffer(const void* data, std::size_t size, Usage usage)
		: m_size(size), m_usage(usage)
	{
		core::verify(not(data == nullptr and usage == Usage::Const), "Cannot create an empty const buffer. Specify data or change usage.");

		const int gl_version = Context::getCurrent().getVersion();

		// OpenGL 4.5 
		if (gl_version >= 45)
		{
			glCreateBuffers(1, &m_id);

			if (usage == Usage::Const)
				// For immutable buffer
				glNamedBufferStorage(m_id, size, data, 0);
			else
			{
				// For mutable buffer
				GLenum gl_usage;
				switch (usage)
				{
				case Usage::Static:  gl_usage = GL_STATIC_DRAW; break;
				case Usage::Dynamic: gl_usage = GL_DYNAMIC_DRAW; break;
				case Usage::Stream:  gl_usage = GL_STREAM_DRAW; break;
				}
				glNamedBufferData(m_id, size, data, gl_usage);
			}
		}
		// OpenGL 3.3 
		else
		{
			glCall(glGenBuffers, 1, &m_id);
			bind();

			GLenum gl_usage;
			switch (usage)
			{
			case Usage::Const:
			case Usage::Static:  gl_usage = GL_STATIC_DRAW; break;
			case Usage::Dynamic: gl_usage = GL_DYNAMIC_DRAW; break;
			case Usage::Stream:  gl_usage = GL_STREAM_DRAW; break;
			}

			glCall(glBufferData, GL_ARRAY_BUFFER, size, data, gl_usage);
		}
		core::log::trace("VertexBuffer {} created", m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& oth) noexcept
		: m_id(std::exchange(oth.m_id, 0)),
		m_size(std::exchange(oth.m_size, 0)),
		m_usage(oth.m_usage),
		m_layout(std::move(oth.m_layout))
	{
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& oth) noexcept
	{
		if (this != &oth)
		{
			if (m_id != 0)
				destroy();
			m_id = std::exchange(oth.m_id, 0);
			m_size = std::exchange(oth.m_size, 0);
			m_usage = oth.m_usage;
			m_layout = std::move(oth.m_layout);
		}
		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		destroy();
	}

	void VertexBuffer::setData(const void* data, std::size_t size, std::size_t offset)
	{
		core::verify(m_usage != Usage::Const, "Cannot setData in const VertexBuffer");
		core::verify(offset + size <= m_size,
			"Buffer overflow");

		const auto gl_version = Context::getCurrent().getVersion();

		if (gl_version >= 45)
			glCall(glNamedBufferSubData, m_id, offset, size, data);
		else
		{
			bind();
			glCall(glBufferSubData, GL_ARRAY_BUFFER, offset, size, data);
		}
	}

	void VertexBuffer::destroy()
	{
		glCall(glDeleteBuffers, 1, &m_id);
		core::log::trace("VertexBuffer {} destroyed", m_id);
	}

	void VertexBuffer::bind() const
	{
		glCall(glBindBuffer, GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind()
	{
		glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(const unsigned int* data, std::size_t count)
		: m_count(count)
	{
		int gfx_version = Context::getCurrent().getVersion();
		glCall(glCreateBuffers, 1, &m_id);
		glCall(glNamedBufferData, m_id, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
		core::log::trace("IndexBuffer {} created", m_id);
	}

	IndexBuffer::~IndexBuffer()
	{
		destroy();
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& oth) noexcept
		: m_id(std::exchange(oth.m_id, 0)), 
		m_count(std::exchange(oth.m_count, 0))
	{
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& oth) noexcept
	{
		if (this != &oth)
		{
			if(m_id != 0)
				destroy();
			m_id = std::exchange(oth.m_id, 0);
			m_count = std::exchange(oth.m_count, 0);
		}
		return *this;
	}

	void IndexBuffer::destroy()
	{
		glCall(glDeleteBuffers, 1, &m_id);
		core::log::trace("IndexBuffer {} destroyed", m_id);
	}

	void IndexBuffer::bind() const
	{
		glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() const
	{
		glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::setData(const unsigned int* data, std::size_t count, std::size_t offset)
	{
		glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_id);
		glCall(glBufferSubData, GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int), count * sizeof(unsigned int), data);
	}

}