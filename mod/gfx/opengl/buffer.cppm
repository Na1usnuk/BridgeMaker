export module bm.gfx:buffer;

import bm.core;

import :shader;
import :utility;

import std;

namespace bm::gfx
{

	export template<typename T>
		concept IndexType = std::is_integral_v<T> and (sizeof(T) == 1 or sizeof(T) == 2 or sizeof(T) == 4);

	export struct VertexLayout
	{
		enum class Type
		{
			Float,
			Float2,
			Float3,
			Float4,

			Int,
			Int2,
			Int3,
			Int4,

			UInt,
			UInt2,
			UInt3,
			UInt4,

			Bool,
		};

		struct Attribute { Type type; bool normalized = false; };

		std::vector<Attribute> attributes;
	};

	export class VertexBuffer
	{
	public:

		VertexBuffer(const void* data, std::size_t size, Usage usage = Usage::Const);
		VertexBuffer(std::size_t size, Usage usage = Usage::Static) : VertexBuffer(nullptr, size, usage) {}
		template<std::ranges::contiguous_range R>
			requires std::is_trivially_copyable_v<std::ranges::range_value_t<R>>
		VertexBuffer(const R& range, Usage usage = Usage::Const) : 
			VertexBuffer(std::ranges::cdata(range), std::ranges::size(range) * sizeof(std::ranges::range_value_t<R>), usage) 
		{}

		~VertexBuffer();
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& oth) noexcept;
		VertexBuffer& operator=(VertexBuffer&& oth) noexcept;

		void bind() const noexcept;
		static void unbind();

		void setData(const void* data, std::size_t size, std::size_t offset_bytes = 0);
		template<std::ranges::contiguous_range R>
			requires std::is_trivially_copyable_v<std::ranges::range_value_t<R>>
		void setData(const R& range, std::size_t offset = 0) 
		{ setData(std::ranges::data(range), std::ranges::size(range) * sizeof(std::ranges::range_value_t<R>), offset); }

		std::size_t getSize() const noexcept { return m_size; }

	private:

		void destroy();

	private:

		unsigned int m_id;
		std::size_t m_size; // size in bytes
		Usage m_usage;

	};


	export class IndexBuffer
	{
	public:

		IndexBuffer(const unsigned int* data, std::size_t count, Usage usage = Usage::Const);

		IndexBuffer(std::size_t count, Usage usage = Usage::Static) :
			IndexBuffer(nullptr, count, usage)
		{}

		IndexBuffer(std::span<const unsigned int> data, Usage usage = Usage::Const) : IndexBuffer(data.data(), data.size(), usage) {}

		~IndexBuffer();
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&& oth) noexcept;
		IndexBuffer& operator=(IndexBuffer&& oth) noexcept;

		void bind() const noexcept;
		static void unbind();
		unsigned int getId() const noexcept { return m_id; }

		void setData(const unsigned int* data, std::size_t count, std::size_t offset_count = 0);
		void setData(std::span<const unsigned int> data, std::size_t offset = 0) { setData(data.data(), data.size(), offset); }

		std::size_t getCount() const noexcept { return m_count; }

	private:

		void destroy();

	private:

		unsigned int m_id;
		std::size_t m_count; // count of elements
		Usage m_usage;

	};
	

	
}