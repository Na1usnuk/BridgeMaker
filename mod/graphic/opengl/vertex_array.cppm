export module bm.gfx:array;

import :buffer;

import bm.core;

import std;

namespace bm::gfx
{
	
	export class VertexArray
	{
	public:
	
		explicit VertexArray(VertexBuffer&& vbo);
		VertexArray(VertexBuffer&& vbo, IndexBuffer&& ibo);
		~VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray(VertexArray&& oth) noexcept;
		VertexArray& operator=(VertexArray&& oth) noexcept;
	
		void setVertexBuffer(VertexBuffer&& vbo);
		void setIndexBuffer(IndexBuffer&& ibo);
		void setLayout(const VertexBuffer::Layout& layout);
	
		void bind() const;
		void unbind() const;
		unsigned int getID() const { return m_id; }
	
		std::size_t getVerticesCount() const { return m_vertices_count; }

		const VertexBuffer& getVertexBuffer() const { return m_vbo; }
		const std::optional<IndexBuffer>& getIndexBuffer() const { return m_ibo; }

		VertexBuffer& getVertexBuffer() { return m_vbo; }
		std::optional<IndexBuffer>& getIndexBuffer() { return m_ibo; }
	
	private:
	
		void init();
		void destroy();
		void tieIBO();

	private:

		unsigned int m_id = 0;
		std::size_t m_vertices_count;
	
		std::size_t m_attrib_index = 0;
	
		// VertexArray owns vbo and ibo
		VertexBuffer m_vbo; 
		std::optional<IndexBuffer> m_ibo;
	
	};

}
