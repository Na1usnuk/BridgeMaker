#include "pch.hpp"

#include "opengl/index_buffer.hpp"
#include "opengl/opengl.hpp"

BM_START
GL_START

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned long long count)
	: m_count(count)
{
	GLCALL(glGenBuffers(1, &m_id));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(count * sizeof(unsigned int)), data, GL_STATIC_DRAW));

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
	GLCALL(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

GL_END
BM_END
