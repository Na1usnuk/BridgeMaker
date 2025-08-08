#include "pch.hpp"

#include "opengl/vertex_array.hpp"
#include "opengl/opengl.hpp"


VertexArray::VertexArray()
	: m_id(0)
{
	GLCALL(glGenVertexArrays(1, &m_id));
	bind();
}

VertexArray::~VertexArray()
{
}

void VertexArray::destroy()
{
	GLCALL(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::addVertexBuffer(const VertexBuffer& vb, const VertexBuffer::Layout& layout)
{
	bind();
	vb.bind();

	const auto& elements = layout.elements();
	unsigned long long offset = 0;

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.stride(), (const void*)offset));
		offset += element.count * VertexBuffer::Layout::Element::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	GLCALL(glBindVertexArray(m_id));
}

void VertexArray::unbind() const
{
	GLCALL(glBindVertexArray(0));
}
