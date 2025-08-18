#pragma once

#include "core.hpp"

#include <memory>
#include <utility>

BM_START

template<typename Backend>
class AbstractRenderer
{
public:

	using PolygonMode              = typename Backend::PolygonMode;
							       
	using VertexArray              = typename Backend::VertexArray;
	using VertexBuffer             = typename Backend::VertexBuffer;
	using VertexBufferLayout       = typename Backend::VertexBuffer::Layout;
	using IndexBuffer              = typename Backend::IndexBuffer;
	using Shader                   = typename Backend::Shader;
							       
							       
	using VertexArrayPtr           = std::shared_ptr<VertexArray>;
	using VertexBufferPtr          = std::shared_ptr<VertexBuffer>;
	using VertexBufferLayoutPtr    = std::shared_ptr<VertexBufferLayout>;
	using IndexBufferPtr           = std::shared_ptr<IndexBuffer>;
	using ShaderPtr                = std::shared_ptr<Shader>;

public:

	AbstractRenderer() {}
	~AbstractRenderer() {}

	void draw(const VertexArray& vao, const IndexBuffer& ebo, const Shader& sh) { m_impl.draw(vao, ebo, sh); }

	void setBackgroundColor(std::array<float, 4> color) { m_impl.setBackgroundColor(color); }
	void setBackgroundColor(std::array<float, 3> color) { m_impl.setBackgroundColor(color); }
	void setPolygonMode(PolygonMode mode = PolygonMode::Fill) { m_impl.setPolygonMode(mode); }
	void setView(std::array<int, 4> viewport) { m_impl.setView(viewport); }
	void clearColor(std::array<float, 4> color) { m_impl.clearColor(color); }
	void clearColor(std::array<float, 3> color) { m_impl.clearColor(color); }
	void clear() { m_impl.clear(); }

	template<typename ...T>
	VertexArrayPtr createVAO(T&&... args) { return std::make_shared<VertexArray>(std::forward<T>(args)...); }
	template<typename ...T>
	VertexBufferPtr createVBO(T&&... args) { return std::make_shared<VertexBuffer>(std::forward<T>(args)...); }
	template<typename ...T>
	VertexBufferLayoutPtr createVBOLayout(T&&... args) { return std::make_shared<VertexBufferLayout>(std::forward<T>(args)...); }
	template<typename ...T>
	IndexBufferPtr createIBO(T&&... args) { return std::make_shared<IndexBuffer>(std::forward<T>(args)...); }
	template<typename ...T>
	ShaderPtr createShader(T&&... args) { return std::make_shared<Shader>(std::forward<T>(args)...); }


private:

	Backend m_impl;

};

BM_END