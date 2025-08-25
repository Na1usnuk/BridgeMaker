#include "pch.hpp"

#include "opengl/vertex_buffer.hpp"
#include "opengl/opengl.hpp"

BM_START
GL_START


VertexBuffer::VertexBuffer(unsigned long long size, Draw draw_hint) : m_size(size_t(size))
{
	GLCALL(glGenBuffers(1, &m_id));
	bind();
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, (int)draw_hint));
}

VertexBuffer::VertexBuffer(const void* data, unsigned long long size, Draw draw_hint) : m_size(size_t(size))
{
	GLCALL(glGenBuffers(1, &m_id));
	bind();
	GLCALL(glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(size), data, (int)draw_hint));
}

VertexBuffer::~VertexBuffer()
{
	destroy();
}

void VertexBuffer::destroy()
{
	if(m_id != 0)
	{
		GLCALL(glDeleteBuffers(1, &m_id));
		m_id = 0;
	}
}

void VertexBuffer::populate(void* data)
{
	bind();
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, m_size, data););
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

GL_END
BM_END
