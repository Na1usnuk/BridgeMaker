export module bm.gfx.vertexarray;

import bm.gfx.buffer.vertex;

import std;

namespace bm::gfx
{

export class VertexArray
{
public:

	enum class DrawAs
	{
		TRIANGLES = 0x0004,
		LINES = 0x0001,
	};

	using Ptr = std::shared_ptr<VertexArray>;
	using KPtrRef = const Ptr&;

public:

	VertexArray(DrawAs as = DrawAs::TRIANGLES);
	VertexArray(VertexBufferPtr vbo, DrawAs as = DrawAs::TRIANGLES);
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& oth) noexcept;
	VertexArray& operator=(VertexArray&& oth) noexcept;

	void addVertexBuffer(VertexBufferPtr vbo);

	void bind() const;
	void unbind() const;
	void destroy();

	void setDrawAs(DrawAs as) { m_draw_as = as; }
	DrawAs getDrawAs() const { return m_draw_as; }

	std::size_t getVerticesCount() const { return m_vertices_count; }

	template<typename... Args>
	static Ptr make(Args&&... args) { return std::make_shared<VertexArray>(std::forward<Args>(args)...); }

private:

	unsigned int m_id = 0;
	std::size_t m_vertices_count;

	DrawAs m_draw_as = DrawAs::TRIANGLES;

};

export using VertexArrayPtr = VertexArray::Ptr;

}
