module;

#include "glad/glad.h"

module bm.gfx.buffer.vertex;


import bm.gfx.utility;

namespace bm::gfx
{

VertexBuffer::VertexBuffer(const void* data, unsigned long long size)
{
	glCall(glGenBuffers, 1, &m_id);
	glCall(glBindBuffer, GL_ARRAY_BUFFER, m_id);
	glCall(glBufferData, GL_ARRAY_BUFFER, GLsizeiptr(size), data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::destroy()
{
	glCall(glDeleteBuffers, 1, &m_id);
}

void VertexBuffer::bind() const
{
	glCall(glBindBuffer, GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
	glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
}


unsigned int VertexBufferLayout::Element::getSizeOfType(unsigned int type)
{
	switch (type)
	{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
	}
	core::verify(false, "Not existing type");
	return 0;
}



}

