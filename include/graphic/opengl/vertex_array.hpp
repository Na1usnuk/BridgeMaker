#pragma once

#include "opengl/vertex_buffer.hpp"

class VertexArray
{
private:

public:

	VertexArray();
	~VertexArray();

	void addVertexBuffer(const VertexBuffer& vb, const VertexBuffer::Layout& layout);

	void bind() const;
	void unbind() const;
	void destroy();

private:

	unsigned int m_id;

};