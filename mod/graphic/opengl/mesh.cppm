export module bm.gfx.mesh;

import bm.gfx.shader;
import bm.gfx.buffer.index;
import bm.gfx.vertexarray;

import std;

namespace bm::gfx
{

export class Mesh
{
public:

	using Ptr = std::shared_ptr<Mesh>;
	using KPtrRef = const Ptr&;

public:

	Mesh(VertexArray::KPtrRef vao, IndexBuffer::KPtrRef ibo, Shader::KPtrRef shader)
		: m_vao(vao), m_ibo(ibo), m_shader(shader)
	{
	}

	Mesh() = default;

	void setVertexArray(VertexArray::KPtrRef vao) { m_vao = vao; }
	void setShader(Shader::KPtrRef shader) { m_shader = shader; }
	void setIndexBuffer(IndexBuffer::KPtrRef ibo) { m_ibo = ibo; }

	std::shared_ptr<VertexArray> getVertexArray() const { return m_vao; }
	std::shared_ptr<Shader>      getShader()      const { return m_shader; }
	std::shared_ptr<IndexBuffer> getIndexBuffer() const { return m_ibo; }

	static Ptr make() { return std::make_shared<Mesh>(); }
	static Ptr make(VertexArray::KPtrRef vao, IndexBuffer::KPtrRef ibo, Shader::KPtrRef shader) { return std::make_shared<Mesh>(vao, ibo, shader); }

private:

	ShaderPtr m_shader = nullptr;
	VertexArrayPtr m_vao = nullptr;
	IndexBufferPtr m_ibo = nullptr;

};

export using MeshPtr = Mesh::Ptr;

}