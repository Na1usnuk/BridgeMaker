#include "pch.hpp"

#include "opengl/vertex_buffer.hpp"
#include "opengl/opengl.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned long long size)
{
	GLCALL(glGenBuffers(1, &m_id));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(size), data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::destroy()
{
	GLCALL(glDeleteBuffers(1, &m_id));
}

void VertexBuffer::bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::unbind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


unsigned int VertexBuffer::Layout::Element::getSizeOfType(unsigned int type)
{
	switch (type)
	{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
	}
	BM_CORE_ASSERT(false, "Not existing type")
	return 0;
}

template<>
void VertexBuffer::Layout::push<float>(unsigned int count)
{
	m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_stride += count * Element::getSizeOfType(GL_FLOAT);
}

template<>
void VertexBuffer::Layout::push<unsigned int>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	m_stride += count * Element::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBuffer::Layout::push<unsigned char>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	m_stride += count * Element::getSizeOfType(GL_UNSIGNED_BYTE);
}
