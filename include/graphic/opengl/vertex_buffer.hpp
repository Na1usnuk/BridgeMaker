#pragma once

#include "core.hpp"
#include "log.hpp"

BM_START
GL_START

class VertexBuffer
{
public:

	class Layout;

public:

	VertexBuffer(const void* data, unsigned long long size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void destroy();

private:

	unsigned int m_id;
};



class VertexBuffer::Layout
{
public:

	struct Element
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int getSizeOfType(unsigned int type);
	};

public:

	Layout() : m_stride(0) {}

	inline unsigned int stride() const { return m_stride; }
	inline const std::vector<Element>& elements() const { return m_elements; }

	template<typename T>
	void push(unsigned int count)
	{
		BM_CORE_ASSERT(false, "This type don`t supported");
	}

	template<>
	void push<float>(unsigned int count);

	template<>
	void push<unsigned int>(unsigned int count);

	template<>
	void push<unsigned char>(unsigned int count);

private:

	std::vector<Element> m_elements;
	unsigned int m_stride;
};

GL_END
BM_END
