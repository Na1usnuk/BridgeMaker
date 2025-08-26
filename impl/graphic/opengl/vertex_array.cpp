module;

#include "glad/glad.h"

module bm.gfx.vertexarray;

import bm.gfx.utility;
import bm.log;
import bm.gfx.buffer.vertex;


namespace bm::gfx
{

VertexArray::VertexArray()
	: m_id(0)
{
	glCall(glGenVertexArrays, 1, &m_id);
	bind();
}

VertexArray::VertexArray(std::shared_ptr<VertexBuffer> vbo, std::shared_ptr<VertexBufferLayout> layout) : VertexArray()
{
	addVertexBuffer(vbo, layout);
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

void VertexArray::addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();

	const auto& elements = layout.elements();
	unsigned long long offset = 0;

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glCall(glEnableVertexAttribArray, i);
		glCall(glVertexAttribPointer, i, element.count, element.type, element.normalized, layout.stride(), (const void*)offset);
		offset += element.count * VertexBufferLayout::Element::getSizeOfType(element.type);
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

