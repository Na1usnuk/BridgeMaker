export module bm.graphic;

export import bm.gfx.shader;
export import bm.gfx.buffer.index;
export import bm.gfx.buffer.vertex;
export import bm.gfx.vertexarray;
export import bm.gfx.texture;
export import bm.gfx.renderer;
export import bm.gfx.utility;
export import bm.gfx.mesh;
export import bm.gfx.geometry;

import std;

export namespace bm::gfx
{

using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
using VertexArrayPtr = std::shared_ptr<VertexArray>;
using ShaderPtr = std::shared_ptr<Shader>;
using LayoutPtr = std::shared_ptr <VertexBufferLayout>;
using TexturePtr = std::shared_ptr<Texture>;

template<Buffer B>
VertexBufferPtr makeVBO(const B& container, VertexBuffer::Draw draw_hint = VertexBuffer::Draw::STATIC) { return std::make_shared<VertexBuffer>(container, draw_hint); }
template<Data D>
VertexBufferPtr makeVBO(D data, std::size_t size, VertexBuffer::Draw draw_hint = VertexBuffer::Draw::STATIC) { return std::make_shared<VertexBuffer>(data, size, draw_hint); }
VertexBufferPtr makeVBO(std::size_t size, VertexBuffer::Draw draw_hint = VertexBuffer::Draw::STATIC) { return std::make_shared<VertexBuffer>(size, draw_hint); }
template<Numeric T>
VertexBufferPtr makeVBO(const std::initializer_list<T>& data, VertexBuffer::Draw draw_hint = VertexBuffer::Draw::STATIC) { return std::make_shared<VertexBuffer>(std::data(data), data.size(), draw_hint); }

template<Buffer B>
IndexBufferPtr makeIBO(const B& container) { return std::make_shared<IndexBuffer>(container); }
IndexBufferPtr makeIBO(const unsigned int* data, std::size_t count) { return std::make_shared<IndexBuffer>(data, count); }
IndexBufferPtr makeIBO(const std::initializer_list<unsigned int>& data) { return std::make_shared<IndexBuffer>(std::data(data), data.size()); }

ShaderPtr makeShader(const std::filesystem::path& filepath) { return std::make_shared<Shader>(filepath); }
ShaderPtr makeShader(std::string_view vertex_source, std::string_view fragment_source) { return std::make_shared<Shader>(vertex_source, fragment_source); }

std::shared_ptr<VertexArray> makeVAO() { return std::make_shared<VertexArray>(); }
std::shared_ptr<VertexArray> makeVAO(std::shared_ptr<VertexBuffer> vbo) { return std::make_shared<VertexArray>(vbo); }

}