module;

#include "glad/glad.h"

export module bm.gfx.buffer.vertex;


import bm.assert;
import bm.log;
import bm.gfx.utility;

import std;

namespace bm::gfx
{

export class VertexBuffer
{
public:

	enum class Draw
	{
		// Magic numbers from opengl
		STATIC = 0x88E4,
		DYNAMIC = 0x88E8,
	};

public:

	VertexBuffer(const void* data, std::size_t size, Draw draw_hint = Draw::STATIC);
	VertexBuffer(std::size_t size, Draw draw_hint = Draw::STATIC) : VertexBuffer(nullptr, size, draw_hint) {}
	template<BufferConcept ContainerType>
	VertexBuffer(const ContainerType& data, Draw draw_hint = Draw::STATIC) : VertexBuffer(data.data(), data.size() * sizeof(typename ContainerType::value_type), draw_hint) {}
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void destroy();
	void populate(const void* data);

private:

	unsigned int m_id;
	std::size_t m_size;

};



export class VertexBufferLayout
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

	VertexBufferLayout() : m_stride(0) {}

	inline unsigned int stride() const { return m_stride; }
	inline const std::vector<Element>& elements() const { return m_elements; }

	template<typename T>
	void push(unsigned int count)
	{
		::bm::core::verify(false, "This type don`t supported");
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * Element::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * Element::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * Element::getSizeOfType(GL_UNSIGNED_BYTE);
	}

private:

	std::vector<Element> m_elements;
	unsigned int m_stride;
};

}

