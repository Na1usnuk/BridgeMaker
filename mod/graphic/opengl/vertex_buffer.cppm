module;

#include "glad/glad.h"

export module bm.gfx.buffer.vertex;


import bm.assert;
import bm.log;
import bm.gfx.utility;

import std;

namespace bm::gfx
{

class VertexBufferLayout
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


export class VertexBuffer
{
public:

	enum class Draw
	{
		// Magic numbers from opengl
		STATIC = 0x88E4,
		DYNAMIC = 0x88E8,
	};

	using Ptr = std::shared_ptr<VertexBuffer>;
	using KPtrRef = const Ptr&;

public:

	//template<Data D>
	VertexBuffer(const void* data, std::size_t size, Draw draw_hint = Draw::STATIC)
		: m_size(size)
	{
		glCall(glGenBuffers, 1, &m_id);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, m_id);
		glCall(glBufferData, GL_ARRAY_BUFFER, size, data, static_cast<int>(draw_hint));
	}
	VertexBuffer(std::size_t size, Draw draw_hint = Draw::STATIC) : VertexBuffer(nullptr, size, draw_hint) {}
	template<Buffer B>
	VertexBuffer(const B& data, Draw draw_hint = Draw::STATIC) : VertexBuffer(std::data(data), data.size() * sizeof(B::value_type), draw_hint) {}
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void destroy();
	void populate(const void* data);
	std::size_t size() const { return m_size; }

	template<typename T>
	void pushLayout(std::size_t count) { m_layout.push<T>(count); }

	const VertexBufferLayout& getLayout() const { return m_layout; }

	template<Data D>
	static Ptr make(D data, std::size_t size, Draw draw_hint = Draw::STATIC) { return std::make_shared<VertexBuffer>(data, size, draw_hint); }
	static Ptr make(std::size_t size, Draw draw_hint = Draw::STATIC) { return std::make_shared<VertexBuffer>(size, draw_hint); }
	template<Buffer B>
	static Ptr make(const B& data, Draw draw_hint = Draw::STATIC) { return std::make_shared<VertexBuffer>(data, draw_hint); }
	template<Numeric N>
	static Ptr make(const std::initializer_list<N>& data, Draw draw_hint = Draw::STATIC) {return VertexBuffer::make<std::initializer_list<N>>(data, draw_hint);}

private:

	unsigned int m_id;
	std::size_t m_size;

	VertexBufferLayout m_layout;

};


export using VertexBufferPtr = VertexBuffer::Ptr;

}

