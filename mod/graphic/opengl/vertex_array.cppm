export module bm.gfx.vertexarray;

import bm.gfx.buffer.vertex;
import bm.gfx.buffer.index;
import bm.traits;

import std;

namespace bm::gfx
{

export class VertexArray
{
public:

	VertexArray();
	VertexArray(Traits<VertexBuffer>::Ptr vbo, Traits<IndexBuffer>::Ptr ibo);
	VertexArray(Traits<VertexBuffer>::Ptr vbo);
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& oth) noexcept;
	VertexArray& operator=(VertexArray&& oth) noexcept;

	void setVertexBuffer(Traits<VertexBuffer>::Ptr vbo);
	void setIndexBuffer(Traits<IndexBuffer>::Ptr ibo);

	void bind() const;
	void unbind() const;
	void destroy();

	std::size_t getVerticesCount() const { return m_vertices_count; }
	Traits<VertexBuffer>::KPtrRef getVertexBuffer() const { return m_vbo; }
	Traits<IndexBuffer>::KPtrRef getIndexBuffer() const { return m_ibo; }

	template<typename... Args>
	static Traits<VertexArray>::Ptr make(Args&&... args) { return std::make_unique<VertexArray>(std::forward<Args>(args)...); }

private:

	unsigned int m_id = 0;
	std::size_t m_vertices_count;

	// VertexArray owns vbo and ibo
	Traits<VertexBuffer>::Ptr m_vbo = nullptr; 
	Traits<IndexBuffer>::Ptr m_ibo = nullptr;

};

export using VertexArrayPtr = Traits<VertexArray>::Ptr;

}
