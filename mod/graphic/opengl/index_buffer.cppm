export module bm.gfx.buffer.index;

import bm.gfx.utility;
import bm.utility;
import bm.traits;

import std;

namespace bm::gfx
{

	export class IndexBuffer
	{
	public:
	
		IndexBuffer(const unsigned int* data, std::size_t count);
	
		IndexBuffer(std::size_t count) : 
			IndexBuffer(nullptr, count) 
		{}
	
		template<Container C>
		IndexBuffer(const C& data) : IndexBuffer(std::data(data), std::size(data)) {}
	
	
		~IndexBuffer();
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&& oth) noexcept;
		IndexBuffer& operator=(IndexBuffer&& oth) noexcept;
	
		void destroy();
	
		void bind() const;
		void unbind() const;
		unsigned int id() const { return m_id; } // Please dont try to destroy object using this id (look destructor).
	
		void setData(const unsigned int* data, std::size_t count, std::size_t offset = 0);
	
		unsigned int count() const { return m_count; }
	
		template<typename... Args>
		static Traits<IndexBuffer>::Ptr make(Args&&... args) { return std::make_unique<IndexBuffer>(std::forward<Args>(args)...); }
	
	private:
	
		unsigned int m_id = 0; // OpenGL obj id
		unsigned long long m_count = 0; // Count of elements to show
	};

}

