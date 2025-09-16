module;

#include "glad/glad.h"

module bm.gfx.vertexarray;

import bm.gfx.utility;
import bm.log;
import bm.gfx.buffer.vertex;


namespace bm::gfx
{

VertexArray::VertexArray(DrawAs as)
	: m_id(0), m_vertices_count(0), m_draw_as(as)
{
	glCall(glGenVertexArrays, 1, &m_id);
	bind();
}

VertexArray::VertexArray(std::shared_ptr<VertexBuffer> vbo, DrawAs as) : VertexArray(as)
{
	addVertexBuffer(vbo);
}

VertexArray::~VertexArray()
{
	destroy();
}

VertexArray::VertexArray(VertexArray&& oth) noexcept
{
	if (m_id == oth.m_id)
		return;
	m_id = oth.m_id;
	oth.m_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& oth) noexcept
{
	if (m_id == oth.m_id)
		return *this;
	m_id = oth.m_id;
	oth.m_id = 0;
	return *this;
}

void VertexArray::destroy()
{
	glCall(glDeleteVertexArrays, 1, &m_id);
	log::core::trace("VertexArray {0} deleted", m_id);
}

void VertexArray::addVertexBuffer(const VertexBuffer& vb)
{
	bind();
	vb.bind();

	const auto& elements = vb.getLayout().elements();
	unsigned long long offset = 0;

	m_vertices_count = vb.size() / elements[0].count;

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glCall(glEnableVertexAttribArray, i);
		glCall(glVertexAttribPointer, i, element.count, element.type, element.normalized, vb.getLayout().stride(), (const void*)offset);
		offset += element.count * elements[0].getSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	glCall(glBindVertexArray, m_id);
}

void VertexArray::unbind() const
{
	glCall(glBindVertexArray, 0);
}

}

