export module bm.gfx.vertexarray;

import bm.gfx.buffer.vertex;

import std;

namespace bm::gfx
{

export class VertexArray
{
private:

public:

	VertexArray();
	VertexArray(std::shared_ptr<VertexBuffer> vbo, std::shared_ptr<VertexBufferLayout> layout);
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& oth) noexcept;
	VertexArray& operator=(VertexArray&& oth) noexcept;

	void addVertexBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
	void addVertexBuffer(std::shared_ptr<VertexBuffer> vbo, std::shared_ptr<VertexBufferLayout> layout) { addVertexBuffer(*vbo, *layout); }

	void bind() const;
	void unbind() const;
	void destroy();

private:

	unsigned int m_id = 0;

};

}
