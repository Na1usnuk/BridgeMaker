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

	Mesh(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<IndexBuffer>& ibo, const std::shared_ptr<Shader>& shader)
		: m_vao(vao), m_ibo(ibo), m_shader(shader)
	{
	}

	Mesh() = default;

	void setVertexArray(const std::shared_ptr<VertexArray>& vao) { m_vao = vao; }
	void setShader(const std::shared_ptr<Shader>& shader) { m_shader = shader; }
	void setIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo) { m_ibo = ibo; }

	std::shared_ptr<VertexArray> getVertexArray() const { return m_vao; }
	std::shared_ptr<Shader>      getShader()      const { return m_shader; }
	std::shared_ptr<IndexBuffer> getIndexBuffer() const { return m_ibo; }

private:

	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<VertexArray> m_vao = nullptr;
	std::shared_ptr<IndexBuffer> m_ibo = nullptr;

};

}