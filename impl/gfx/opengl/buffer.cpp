module;

#include <glad/glad.h>

#define GL_VERIFY(func, ...) glVerify( std::source_location::current(), func, __VA_ARGS__)

module bm.gfx:buffer;

import :verify;
import :buffer;
import :shader;
import :context;

import bm.core;

namespace bm::gfx
{

	// Helper for buffer creation
	static GLuint makeBuffer(GLenum type, const void* data, std::size_t size, Usage usage)
	{
		const int gl_version = Context::getCurrent().getVersion();
		GLuint id = 0;

		// OpenGL 4.5 
		if (gl_version >= 45)
		{
			glCreateBuffers(1, &id);

			if (usage == Usage::Const)
				// For immutable buffer
				glNamedBufferStorage(id, size, data, 0);
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
				glNamedBufferData(id, size, data, gl_usage);
			}
		}
		// OpenGL 3.3 
		else
		{
			GL_VERIFY(glGenBuffers, 1, &id);
			GL_VERIFY(glBindBuffer, type, id);

			GLenum gl_usage;
			switch (usage)
			{
			case Usage::Const:
			case Usage::Static:  gl_usage = GL_STATIC_DRAW; break;
			case Usage::Dynamic: gl_usage = GL_DYNAMIC_DRAW; break;
			case Usage::Stream:  gl_usage = GL_STREAM_DRAW; break;
			}

			GL_VERIFY(glBufferData, type, size, data, gl_usage);
		}
		return id;
	}

	VertexBuffer::VertexBuffer(const void* data, std::size_t size, Usage usage)
		: m_size(size), m_usage(usage)
	{
		core::verify(not(data == nullptr and usage == Usage::Const), "Cannot create an empty const buffer. Specify data or change usage.");
		m_id = makeBuffer(GL_ARRAY_BUFFER, data, size, usage);
		core::log::trace("VertexBuffer {} created", m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& oth) noexcept
		: m_id(std::exchange(oth.m_id, 0)),
		m_size(std::exchange(oth.m_size, 0)),
		m_usage(oth.m_usage)
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
		core::verify(offset + size <= m_size, "VertexBuffer overflow");

		const auto gl_version = Context::getCurrent().getVersion();

		if (gl_version >= 45)
			glNamedBufferSubData(m_id, offset, size, data);
		else
		{
			bind();
			GL_VERIFY(glBufferSubData, GL_ARRAY_BUFFER, offset, size, data);
		}
	}

	void VertexBuffer::destroy()
	{
		if(m_id != 0)
		{
			core::log::trace("VertexBuffer {} destroyed", m_id);
			GL_VERIFY(glDeleteBuffers, 1, &m_id);
			m_id = 0;
		}
	}

	void VertexBuffer::bind() const noexcept
	{
		GL_VERIFY(glBindBuffer, GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind()
	{
		GL_VERIFY(glBindBuffer, GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(const unsigned int* data, std::size_t count, Usage usage)
		: m_count(count)
	{
		int gfx_version = Context::getCurrent().getVersion();
		m_id = makeBuffer(GL_ELEMENT_ARRAY_BUFFER, data, count * sizeof(unsigned int), usage);
		m_count = count;
		core::log::trace("IndexBuffer {} created", m_id);
	}

	IndexBuffer::~IndexBuffer()
	{
		destroy();
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& oth) noexcept
		: m_id(std::exchange(oth.m_id, 0)),
		m_usage(oth.m_usage)
	{
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& oth) noexcept
	{
		if (this != &oth)
		{
			destroy();
			m_id = std::exchange(oth.m_id, 0);
			m_usage = oth.m_usage;
		}
		return *this;
	}

	void IndexBuffer::destroy()
	{
		if(m_id != 0)
		{
			core::log::trace("IndexBuffer {} destroyed", m_id);
			GL_VERIFY(glDeleteBuffers, 1, &m_id);
			m_id = 0;
		}
	}

	void IndexBuffer::bind() const noexcept
	{ 
		GL_VERIFY(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind()
	{
		GL_VERIFY(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::setData(const unsigned int* data, std::size_t count, std::size_t offset)
	{
		core::verify(m_usage != Usage::Const, "Cannot setData in const IndexBuffer");
		core::verify(offset + count <= m_count, "IndexBuffer overflow");

		m_count = count;
		const auto gl_version = Context::getCurrent().getVersion();

		if (gl_version >= 45)
			glNamedBufferSubData(m_id, offset * sizeof(unsigned int), count * sizeof(unsigned int), data);
		else
		{
			bind();
			GL_VERIFY(glBufferSubData, GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int), count * sizeof(unsigned int), data);
		}
	}

}