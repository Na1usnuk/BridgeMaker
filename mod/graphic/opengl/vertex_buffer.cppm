module;

#include "glad/glad.h"

export module bm.gfx.buffer.vertex;


import bm.verify;
import bm.log;
import bm.gfx.utility;
import bm.utility;
import bm.traits;
import bm.gfx.shader;

import std;

namespace bm::gfx
{

	export class VertexBuffer
	{
	public:

		enum class Usage
		{
			Const,
			Static, // For immutable data
			Dynamic, // Data can change sometimes
			Stream, // Data changes frequently 
		};

		class Data;
		class Layout
		{
		public:

			struct Element
			{
				Element(Shader::Type type_, std::string_view name_, bool normalized_ = false) :
					type(type_),
					gl_type(getGLType(type_)),
					name(name_),
					size(getSize(type_)),
					count(getComponentCount(type_)),
					offset(0),
					normalized(normalized_)
				{
				}

				static constexpr std::uint32_t getSize(Shader::Type type)
				{
					switch (type)
					{
					case Shader::Type::Float:		return sizeof(GLfloat);
					case Shader::Type::Float2:		return sizeof(GLfloat) * 2;
					case Shader::Type::Float3:		return sizeof(GLfloat) * 3;
					case Shader::Type::Float4:		return sizeof(GLfloat) * 4;
					case Shader::Type::Float3x3:    return sizeof(GLfloat) * 3 * 3;
					case Shader::Type::Float4x4:    return sizeof(GLfloat) * 4 * 4;
					case Shader::Type::Int:			return sizeof(GLint);
					case Shader::Type::Int2:		return sizeof(GLint) * 2;
					case Shader::Type::Int3:		return sizeof(GLint) * 3;
					case Shader::Type::Int4:		return sizeof(GLint) * 4;
					case Shader::Type::Bool:		return sizeof(GLboolean);
					}
					core::verify(false, "Not existing VertexBufferLayout::Element type");
					return 0;
				}

				static constexpr std::int32_t getComponentCount(Shader::Type type)
				{
					switch (type)
					{
					case Shader::Type::Float:		return 1;
					case Shader::Type::Float2:		return 2;
					case Shader::Type::Float3:		return 3;
					case Shader::Type::Float4:		return 4;
					case Shader::Type::Float3x3:    return 3 * 3;
					case Shader::Type::Float4x4:    return 4 * 4;
					case Shader::Type::Int:			return 1;
					case Shader::Type::Int2:		return 2;
					case Shader::Type::Int3:		return 3;
					case Shader::Type::Int4:		return 4;
					case Shader::Type::Bool:		return 1;
					}
					core::verify(false, "Not existing VertexBufferLayout::Element type");
					return 0;
				}

				static constexpr std::uint32_t getGLType(Shader::Type type)
				{
					switch (type)
					{
						// Float types
					case Shader::Type::Float:
					case Shader::Type::Float2:
					case Shader::Type::Float3:
					case Shader::Type::Float4:
					case Shader::Type::Float2x2:
					case Shader::Type::Float3x3:
					case Shader::Type::Float4x4: return GL_FLOAT;

						// Signed integers
					case Shader::Type::Int:
					case Shader::Type::Int2:
					case Shader::Type::Int3:
					case Shader::Type::Int4:    return GL_INT;

						// Unsigned integers
					case Shader::Type::UInt:
					case Shader::Type::UInt2:
					case Shader::Type::UInt3:
					case Shader::Type::UInt4:   return GL_UNSIGNED_INT;

						// Boolean
					case Shader::Type::Bool:    return GL_BOOL;
					}
					core::verify(false, "Not existing VertexBufferLayout::Element type");
					return 0;
				}

				Shader::Type type;
				std::uint32_t gl_type;
				std::string name;
				std::uint32_t size;
				std::uint32_t count;
				std::size_t offset;
				bool normalized;
			};

		public:

			Layout() : m_stride(0) {}
			Layout(const std::initializer_list<Element>& layout) :
				m_elements(layout),
				m_stride(0)
			{
				calculateOffsetsAndStride();
			}

			inline unsigned int stride() const { return m_stride; }
			inline const std::vector<Element>& elements() const { return m_elements; }

			void set(const std::initializer_list<Element>& layout) { m_elements = layout; calculateOffsetsAndStride();}
			//void push(const Element& element) { m_elements.push_back(element); }

		private:

			void calculateOffsetsAndStride()
			{
				size_t offset = 0;
				m_stride = 0;
				for (auto& element : m_elements)
				{
					element.offset = offset;
					offset += element.size;
					m_stride += element.size;
				}
			}

		private:

			std::vector<Element> m_elements;
			unsigned int m_stride;
		};

	public:

		VertexBuffer(const void* data, std::size_t size, Usage usage = Usage::Dynamic);
		VertexBuffer(std::size_t size, Usage usage = Usage::Dynamic) : VertexBuffer(nullptr, size, usage) {}
		template<Container C>
		VertexBuffer(const C& data, Usage usage = Usage::Dynamic) : VertexBuffer(std::data(data), std::size(data) * sizeof(C::value_type), usage) {}

		~VertexBuffer();

		void bind() const;
		void unbind() const;
		void destroy();
		void setData(const void* data, std::size_t size, std::size_t offset = 0);
		std::size_t size() const { return m_size; }

		void setLayout(const Layout& layout)
		{
			m_layout = layout;
		}

		//void pushLayout(const Layout::Element& element)
		//{
		//	m_layout.push(element);
		//}

		const Layout& getLayout() const { return m_layout; }

		template<typename... Args>
		static Traits<VertexBuffer>::Ptr make(Args&&... args)
		{
			return std::make_unique<VertexBuffer>(std::forward<Args>(args)...);
		}

	private:

		unsigned int m_id;
		std::size_t m_size;
		Usage m_usage;

		Layout m_layout;

	};

}
	
	

 // To represent all data that VertexBuffer need
//class VertexBuffer::Data
//{
//private:
//
//	enum class LayoutType
//	{
//		FLOAT = 1,
//		UNSIGNED_INT,
//		UNSIGNED_CHAR
//	};
//
//public:
//
//	Data() = default;
//
//
//	template<typename T>
//	void pushLayout(unsigned int count) { core::verify(false, "Type not supported"); }
//	template<>
//	void pushLayout<float>(unsigned int count) { m_layout.push_back({ LayoutType::FLOAT, count }); }
//	template<>
//	void pushLayout<unsigned int >(unsigned int count) { m_layout.push_back({ LayoutType::UNSIGNED_INT, count }); }
//	template<>
//	void pushLayout<unsigned char>(unsigned int count) { m_layout.push_back({ LayoutType::UNSIGNED_CHAR, count }); }
//
//	const std::vector<float>& getBuffer() const { return m_buffer; }
//	const std::vector<std::pair<LayoutType, unsigned int>>& getLayouts() const { return m_layout; }
//	std::size_t getBufferSize() const { return m_buffer.size(); }
//	std::size_t getLayoutsSize() const { return m_layout.size(); }
//
//private:
//
//	std::vector<float> m_buffer;
//	std::vector<std::pair<LayoutType, unsigned int>> m_layout;
//	Draw m_draw_hint;
//
//};
//
//export Traits<VertexBuffer::Data>::Ptr operator+ (Traits<VertexBuffer::Data>::KPtrRef lhs, Traits<VertexBuffer::Data>::KPtrRef rhs)
//{
//	return Traits<VertexBuffer::Data>::Ptr{};
//}