#pragma once

#include "core.hpp"

#include "vertex_buffer.hpp"

BM_START
GL_START

class VertexArray
{
private:

public:

	VertexArray();
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& oth) noexcept;
	VertexArray& operator=(VertexArray&& oth) noexcept;

	void addVertexBuffer(const VertexBuffer& vb, const VertexBuffer::Layout& layout);

	void bind() const;
	void unbind() const;
	void destroy();

private:

	unsigned int m_id = 0;

};

GL_END
BM_END