export module bm.gfx.mesh;

import bm.gfx.shader;
import bm.gfx.buffer.index;
import bm.gfx.vertexarray;

import bm.traits;

import std;

namespace bm::gfx
{

export class Mesh
{
public:

	using Ptr = Traits<Mesh>::Ptr;
	using KPtrRef = Traits<Mesh>::KPtrRef;

	using VAKPtrRef = Traits<VertexArray>::KPtrRef;
	using VAPtr = Traits<VertexArray>::Ptr;
	using IBKPtrRef = Traits<IndexBuffer>::KPtrRef;
	using IBPtr = Traits<IndexBuffer>::Ptr;

public:

	Mesh(VAKPtrRef vao, IBKPtrRef ibo)
		: m_vao(vao), m_ibo(ibo) {}

	Mesh(VAKPtrRef vao)
		: m_vao(vao) {}

	Mesh() = default;

	void setVertexArray(VAKPtrRef vao) { m_vao = vao; }
	void setIndexBuffer(IBKPtrRef ibo) { m_ibo = ibo; }

	VAKPtrRef getVertexArray() const { return m_vao; }
	IBKPtrRef getIndexBuffer() const { return m_ibo; }

	static Ptr make() { return std::make_shared<Mesh>(); }
	static Ptr make(VAKPtrRef vao, IBKPtrRef ibo) { return std::make_shared<Mesh>(vao, ibo); }
	static Ptr make(VAKPtrRef vao) { return std::make_shared<Mesh>(vao); }

private:

	VAPtr m_vao = nullptr;
	IBPtr m_ibo = nullptr;

};

export using MeshPtr = Mesh::Ptr;

}