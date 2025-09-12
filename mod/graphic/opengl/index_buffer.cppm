module;

#include "glad/glad.h"

export module bm.gfx.buffer.index;

import bm.gfx.utility;

import std;

namespace bm::gfx
{

export class IndexBuffer
{
public:

	using Ptr = std::shared_ptr<IndexBuffer>;
	using KPtrRef = const Ptr&;

public:

	IndexBuffer(const unsigned int* data, std::size_t count)
		: m_count(count)
	{
		glCall(glGenBuffers, 1, &m_id);
		glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_id);
		glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	template<Buffer C>
	IndexBuffer(const C& data) : IndexBuffer(std::data(data), data.size()) {}


	~IndexBuffer();
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;
	IndexBuffer(IndexBuffer&& oth) noexcept;
	IndexBuffer& operator=(IndexBuffer&& oth) noexcept;

	void destroy();

	void bind() const;
	void unbind() const;

	unsigned int count() const { return m_count; }

	static Ptr make(const unsigned int* data, std::size_t count) { return std::make_shared<IndexBuffer>(data, count); }
	template<Buffer C>
	static Ptr make(const C& data) { return std::make_shared<IndexBuffer>(data); }
	static Ptr make(const std::initializer_list<unsigned int>& data) { return IndexBuffer::make<std::initializer_list<unsigned int>>(data); }

private:

	unsigned int m_id = 0;
	unsigned long long m_count = 0;
	int m_data_t;
};

export using IndexBufferPtr = IndexBuffer::Ptr;

}

