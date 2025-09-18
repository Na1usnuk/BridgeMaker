module;

#include "glad/glad.h"

module bm.gfx.buffer.vertex;


import bm.gfx.utility;

namespace bm::gfx
{

VertexBuffer::VertexBuffer(const void* data, std::size_t size, Draw draw_hint)
	: m_size(size)
{
	glCall(glCreateBuffers, 1, &m_id);
	glCall(glNamedBufferStorage, m_id, size, data, 0);
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::populate(const void* data)
{
	bind();
	glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, m_size, data);
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

