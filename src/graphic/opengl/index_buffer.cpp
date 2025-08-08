#include "pch.hpp"

#include "opengl/index_buffer.hpp"
#include "opengl/opengl.hpp"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned long long count)
	: m_count(count)
{
	GLCALL(glGenBuffers(1, &m_id));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(count * sizeof(unsigned int)), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
	
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

