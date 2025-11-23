module;

#include "glad/glad.h"

module bm.gfx.buffer.vertex;


import bm.gfx.utility;
import bm.window;

namespace bm::gfx
{

    VertexBuffer::VertexBuffer(const void* data, std::size_t size, Usage usage)
        : m_size(size), m_usage(usage)
    {
        core::verify(not(data == nullptr and usage == Usage::Const), "You cannot create an empty const buffer. Specify data or change usage.");

        const auto gl_version = Context::get().getVersion();

        // OpenGL 4.5 
        if (gl_version >= 45)
        {
            glCall(glCreateBuffers, 1, &m_id);

            if (usage == Usage::Const)
            {
                // For immutable buffer
                glCall(glNamedBufferStorage, m_id, size, data, 0);
            }
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
                glCall(glNamedBufferData, m_id, size, data, gl_usage);
            }
        }
        // OpenGL 3.3 
        else
        {
            glCall(glGenBuffers, 1, &m_id);
            bind();

            GLenum gl_usage;
            switch (usage)
            {
            case Usage::Const:
            case Usage::Static:  gl_usage = GL_STATIC_DRAW; break;
            case Usage::Dynamic: gl_usage = GL_DYNAMIC_DRAW; break;
            case Usage::Stream:  gl_usage = GL_STREAM_DRAW; break;
            }

            glCall(glBufferData, GL_ARRAY_BUFFER, size, data, gl_usage);
        }
    }


    VertexBuffer::~VertexBuffer()
    {
    }
    
    void VertexBuffer::setData(const void* data, std::size_t size, std::size_t offset)
    {
        core::verify(m_usage != Usage::Const, "You create buffer with Const specifier and now call setData? Why?");
        core::verify(offset + size <= m_size,
            "Buffer overflow");

        const auto gl_version = Context::get().getVersion();

        if (gl_version >= 45)
            glCall(glNamedBufferSubData, m_id, offset, size, data);
        else
        {
    	    bind();
    	    glCall(glBufferSubData, GL_ARRAY_BUFFER, offset, size, data);
        }
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

