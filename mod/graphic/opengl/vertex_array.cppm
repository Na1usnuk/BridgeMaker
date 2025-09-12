export module bm.gfx.vertexarray;

import bm.gfx.buffer.vertex;

import std;

namespace bm::gfx
{

export class VertexArray
{
public:

	using Ptr = std::shared_ptr<VertexArray>;
	using KPtrRef = const Ptr&;

public:

	VertexArray();
	VertexArray(VertexBufferPtr vbo);
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& oth) noexcept;
	VertexArray& operator=(VertexArray&& oth) noexcept;

	void addVertexBuffer(const VertexBuffer& vbo);
	void addVertexBuffer(VertexBufferPtr vbo) { addVertexBuffer(*vbo); }

	void bind() const;
	void unbind() const;
	void destroy();

	static Ptr make() { return std::make_shared<VertexArray>(); }
	static Ptr make(VertexBufferPtr vbo) { return std::make_shared<VertexArray>(vbo); }

private:

	unsigned int m_id = 0;

};

export using VertexArrayPtr = VertexArray::Ptr;

}
