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

VertexArray::VertexArray()
	: m_id(0), m_vertices_count(0)
{
	GL_CALL(glGenVertexArrays, 1, &m_id);
	bind();
	core::log::trace("VertexArray {} created", m_id);
}

VertexArray::VertexArray(Traits<VertexBuffer>::Ptr vbo) : VertexArray()
{
	setVertexBuffer(std::move(vbo));
}

VertexArray::VertexArray(Traits<VertexBuffer>::Ptr vbo, Traits<IndexBuffer>::Ptr ibo) : VertexArray()
{
	setVertexBuffer(std::move(vbo));
	setIndexBuffer(std::move(ibo));
}

VertexArray::~VertexArray()
{
	destroy();
}

VertexArray::VertexArray(VertexArray&& oth) noexcept
{
	auto id = oth.m_id;
	oth.m_id = 0;
	m_id = id;
}

VertexArray& VertexArray::operator=(VertexArray&& oth) noexcept
{
	auto id = oth.m_id;
	oth.m_id = 0;
	m_id = id;
	return *this;
}

void VertexArray::destroy()
{
	GL_CALL(glDeleteVertexArrays, 1, &m_id);
	core::log::trace("VertexArray {0} deleted", m_id);
}

void VertexArray::setIndexBuffer(Traits<IndexBuffer>::Ptr ibo) 
{ 
	m_ibo = std::move(ibo); 
	GL_CALL(glVertexArrayElementBuffer, m_id, m_ibo->id());
}

void VertexArray::setVertexBuffer(Traits<VertexBuffer>::Ptr vbo)
{
	bind();
	m_vbo = std::move(vbo);
	m_vbo->bind();

	const auto& elements = m_vbo->getLayout().elements();
	unsigned long long offset = 0;

	m_vertices_count = m_vbo->size() / m_vbo->getLayout().stride();

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GL_CALL(glEnableVertexAttribArray, m_attrib_index);
		GL_CALL(glVertexAttribPointer, m_attrib_index, element.count, element.gl_type, element.normalized, m_vbo->getLayout().stride(), (const void*)offset);
		offset += element.size;
		++m_attrib_index;
	}
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

