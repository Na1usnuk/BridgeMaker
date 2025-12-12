export module bm.gfx:buffer;

import bm.core;

import :shader;

import std;

namespace bm::gfx
{

	export enum class Usage
	{
		Const,	 // For immutable data
		Static,  // Data changes rarelly
		Dynamic, // Data can change sometimes
		Stream,  // Data changes frequently 
	};

	export class VertexBuffer
	{
	public:

		class Data;
		class Layout
		{
		public:

			struct Element
			{
				Element(Shader::Type type_, std::string_view name_, bool normalized_ = false);

				static constexpr std::uint32_t getSize(Shader::Type type);
				static constexpr std::int32_t getComponentCount(Shader::Type type);
				static constexpr std::uint32_t getGLType(Shader::Type type);

				Shader::Type type;
				std::uint32_t gl_type;
				std::string name;
				std::uint32_t size;
				std::uint32_t count;
				std::size_t offset;
				bool normalized;
			};

		public:

			Layout();
			Layout(const std::initializer_list<Element>& layout);

			inline unsigned int stride() const { return m_stride; }
			inline const std::vector<Element>& elements() const { return m_elements; }

			void set(const std::initializer_list<Element>& layout) { m_elements = layout; calculateOffsetsAndStride(); }
			//void push(const Element& element) { m_elements.push_back(element); }

		private:

			void calculateOffsetsAndStride();

		private:

			std::vector<Element> m_elements;
			unsigned int m_stride;
		};

	public:

		VertexBuffer(const void* data, std::size_t size, Usage usage = Usage::Const);
		VertexBuffer(std::size_t size, Usage usage = Usage::Static) : VertexBuffer(nullptr, size, usage) {}

		template<std::ranges::contiguous_range R>
		VertexBuffer(const R& range, Usage usage = Usage::Const) : VertexBuffer(std::ranges::cdata(range), std::ranges::size(range) * sizeof(std::ranges::range_value_t<R>), usage) {}

		~VertexBuffer();
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& oth) noexcept;
		VertexBuffer& operator=(VertexBuffer&& oth) noexcept;

		void bind() const;
		static void unbind();
		void destroy();
		void setData(const void* data, std::size_t size, std::size_t offset = 0);
		std::size_t size() const { return m_size; }

		void setLayout(const Layout& layout) { m_layout = layout; }

		const Layout& getLayout() const { return m_layout; }

		template<typename... Args>
		static Traits<VertexBuffer>::Ptr make(Args&&... args) { return std::make_unique<VertexBuffer>(std::forward<Args>(args)...); }

	private:

		unsigned int m_id;
		std::size_t m_size;
		Usage m_usage;

		Layout m_layout;
	};


	export class IndexBuffer
	{
	public:

		IndexBuffer(const unsigned int* data, std::size_t count);

		IndexBuffer(std::size_t count) :
			IndexBuffer(nullptr, count)
		{
		}

		IndexBuffer(std::span<const unsigned int> data) : IndexBuffer(data.data(), data.size()) {}


		~IndexBuffer();
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&& oth) noexcept;
		IndexBuffer& operator=(IndexBuffer&& oth) noexcept;

		void destroy();

		void bind() const;
		void unbind() const;
		unsigned int id() const { return m_id; }

		void setData(const unsigned int* data, std::size_t count, std::size_t offset = 0);

		unsigned int count() const { return m_count; }

		template<typename... Args>
		static Traits<IndexBuffer>::Ptr make(Args&&... args) { return std::make_unique<IndexBuffer>(std::forward<Args>(args)...); }

	private:

		unsigned int m_id = 0;
		unsigned long long m_count = 0; // Count of elements to show
	};
	

	
}