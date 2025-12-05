module;

#include "glad/glad.h"
#include "gl_call.hpp"

module bm.gfx.buffer.index;

import bm.gfx.utility;
import std;


namespace bm::gfx
{

	IndexBuffer::IndexBuffer(const unsigned int* data, std::size_t count)
		: m_count(count)
	{
		glCall(glCreateBuffers, 1, &m_id);
		glCall(glNamedBufferData, m_id, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}
	
	IndexBuffer::~IndexBuffer()
	{
		destroy();
	}
	
	IndexBuffer::IndexBuffer(IndexBuffer&& oth) noexcept
	{
		if (m_id == oth.m_id)
			return;
		m_id = oth.m_id;
		m_count = oth.m_count;
		oth.m_id = 0;
	}
	
	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& oth) noexcept
	{
		if (m_id == oth.m_id)
			return *this;
		m_id = oth.m_id;
		m_count = oth.m_count;
		oth.m_id = 0;
		return *this;
	}
	
	void IndexBuffer::destroy()
	{
		glCall(glDeleteBuffers, 1, &m_id);
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
