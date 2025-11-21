module;

#include "glad/glad.h"

module bm.gfx.buffer.vertex;


import bm.gfx.utility;

namespace bm::gfx
{

VertexBuffer::VertexBuffer(const void* data, std::size_t size, Usage usage)
	: m_size(size)
{

    glCall(glGenBuffers, 1, &m_id);
    bind();

    GLenum gl_usage;
    switch (usage) 
    {
    case Usage::Static:  gl_usage = GL_STATIC_DRAW; break;
    case Usage::Dynamic: gl_usage = GL_DYNAMIC_DRAW; break;
    case Usage::Stream:  gl_usage = GL_STREAM_DRAW; break;
    }

    glCall(glBufferData, GL_ARRAY_BUFFER, size, data, gl_usage);

}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::populate(const void* data, std::size_t size, std::size_t offset)
{
	bind();
	glCall(glBufferSubData, GL_ARRAY_BUFFER, offset, size, data);
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



}

