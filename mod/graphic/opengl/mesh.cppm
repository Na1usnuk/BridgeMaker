export module bm.gfx:mesh;

import :array;

import bm.core;

import std;

namespace bm::gfx
{

export class Mesh
{
public:

	enum class DrawAs
	{
		Triangles = 0x0004,
		Lines = 0x0001,
		Points = 0x0000
	};

public:

	Mesh(Traits<VertexArray>::Ptr vao, DrawAs draw_as = DrawAs::Triangles)
		: m_vao(std::move(vao)), m_draw_as(draw_as)
	{}

	Mesh() = default;

	void setVertexArray(Traits<VertexArray>::Ptr vao) { m_vao = std::move(vao); }
	Traits<VertexArray>::KPtrRef getVertexArray() const { return m_vao; }

	DrawAs getDrawAs() const { return m_draw_as; }
	void setDrawAs(DrawAs draw_as) { m_draw_as = draw_as; }

	static Traits<Mesh>::SPtr make() { return std::make_shared<Mesh>(); }
	static Traits<Mesh>::SPtr make(Traits<VertexArray>::Ptr vao, DrawAs draw_as = DrawAs::Triangles) { return std::make_shared<Mesh>(std::move(vao), draw_as); }

private:

	Traits<VertexArray>::Ptr m_vao = nullptr; // Owns vao

	DrawAs m_draw_as = DrawAs::Triangles; // Triangles by default

};

}