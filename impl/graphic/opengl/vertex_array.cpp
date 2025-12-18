module;

#include "glad/glad.h"

#define GL_CALL(func, ...) glCallImpl( std::source_location::current(), func, __VA_ARGS__)

module bm.gfx:array;

import :utility;
import :buffer;
import :array;

import bm.core;

namespace bm::gfx
{

VertexArray::VertexArray(VertexBuffer&& vbo) :
	m_vbo(std::move(vbo)),
	m_ibo(std::nullopt),
	m_id(0),
	m_vertices_count(0)
{
	init();
}

VertexArray::VertexArray(VertexBuffer&& vbo, IndexBuffer&& ibo) : 
	m_vbo(std::move(vbo)),
	m_ibo(std::move(ibo)),
	m_id(0),
	m_vertices_count(0)
{
	init();
	tieIBO();
}

VertexArray::~VertexArray()
{
	destroy();
}

VertexArray::VertexArray(VertexArray&& oth) noexcept : 
	m_id(std::exchange(oth.m_id, 0)),
	m_vbo(std::move(oth.m_vbo)),
	m_ibo(std::move(oth.m_ibo)),
	m_vertices_count(std::exchange(oth.m_vertices_count, 0)),
	m_attrib_index(std::exchange(oth.m_attrib_index, 0))
{
}

VertexArray& VertexArray::operator=(VertexArray&& oth) noexcept
{
	if (this != &oth)
	{
		if (m_id != 0)
			destroy();
		m_id = std::exchange(oth.m_id, 0);
		m_vbo = std::move(oth.m_vbo);
		m_ibo = std::move(oth.m_ibo);
		m_vertices_count = std::exchange(oth.m_vertices_count, 0);
		m_attrib_index = std::exchange(oth.m_attrib_index, 0);
	}
	return *this;
}

void VertexArray::init()
{
	GL_CALL(glCreateVertexArrays, 1, &m_id);
	core::log::trace("VertexArray {} created", m_id);
}

void VertexArray::destroy()
{
	GL_CALL(glDeleteVertexArrays, 1, &m_id);
	core::log::trace("VertexArray {} deleted", m_id);
}

void VertexArray::setIndexBuffer(IndexBuffer&& ibo) 
{ 
	m_ibo = std::move(ibo);
	tieIBO();
}

void VertexArray::tieIBO()
{
	if(m_ibo)
		GL_CALL(glVertexArrayElementBuffer, m_id, m_ibo->id());
}

void VertexArray::setLayout(const VertexBuffer::Layout& layout)
{
	bind();
	m_vbo.bind();

	m_attrib_index = 0;

	const auto& elements = layout.elements();
	unsigned long long offset = 0;

	m_vertices_count = m_vbo.size() / layout.stride();

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GL_CALL(glEnableVertexAttribArray, m_attrib_index);
		GL_CALL(glVertexAttribPointer, m_attrib_index, element.count, element.gl_type, element.normalized, layout.stride(), (const void*)offset);
		offset += element.size;
		++m_attrib_index;
	}
}

void VertexArray::setVertexBuffer(VertexBuffer&& vbo)
{
	m_vbo = std::move(vbo);
}

void VertexArray::bind() const
{
	GL_CALL(glBindVertexArray, m_id);
}

void VertexArray::unbind() const
{
	GL_CALL(glBindVertexArray, 0);
}

}

