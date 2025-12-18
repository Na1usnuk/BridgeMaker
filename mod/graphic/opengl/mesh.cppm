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
	
		Mesh(VertexArray&& vao, DrawAs draw_as = DrawAs::Triangles)
			: m_vao(std::move(vao)), m_draw_as(draw_as)
		{}

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		Mesh(Mesh&&) noexcept = default;
		Mesh& operator=(Mesh&&) noexcept = default;

		~Mesh() = default;
	
		void setVertexArray(VertexArray&& vao) { m_vao = std::move(vao); }
		const VertexArray& getVertexArray() const { return m_vao; }
		VertexArray& getVertexArray() { return m_vao; }
	
		DrawAs getDrawAs() const { return m_draw_as; }
		void setDrawAs(DrawAs draw_as) { m_draw_as = draw_as; }
	
	private:
	
		VertexArray m_vao; // Owns vao
		DrawAs m_draw_as; // Triangles by default
	
	};
	
}