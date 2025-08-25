module;

#include "glad/glad.h"

module bm.gfx.buffer.index;

import bm.gfx.utility;

namespace bm::gfx
{


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned long long count)
	: m_count(count)
{
	glCall(glGenBuffers, 1, &m_id);
	glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_id);
	glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(count * sizeof(unsigned int)), data, GL_STATIC_DRAW);

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


}
